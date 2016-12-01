#include "GameMainWindow.h"

//#define MULTICAST_INTERFACE_REFEREE "eth0"
#define MULTICAST_ADDRESS_REFEREE "224.5.23.1"
#define MULTICAST_PORT_REFEREE 10001 /*NEW PROTOCOL IS ON PORT 10003*/

#define MULTICAST_INTERFACE_SSL_PACKET ""
#define MULTICAST_ADDRESS_SSL_PACKET "224.5.23.2"
#define MULTICAST_PORT_SSL_PACKET 10002

#define GR_SIM_ADDRESS "127.0.0.1"
#define GR_SIM_PORT 20011


GameMainWindow::GameMainWindow(QMainWindow *parent) : QMainWindow(parent)
{
    groupAddressReferee = QHostAddress (MULTICAST_ADDRESS_REFEREE );
    udpSocketReferee = new QUdpSocket(this);

    udpSocketReferee->bind(MULTICAST_PORT_REFEREE, QUdpSocket::ShareAddress);
    udpSocketReferee->joinMulticastGroup(groupAddressReferee);
    connect( udpSocketReferee, SIGNAL ( readyRead() ),  this, SLOT ( processPendingDatagramsReferee() ) );

    udpSocketSslPacket = new QUdpSocket();
    portSslPacket = MULTICAST_PORT_SSL_PACKET;
    netAddressSslPacket = new QHostAddress ( QString ( MULTICAST_ADDRESS_SSL_PACKET ) );
    netInterfaceSslPacket  = new QNetworkInterface ( QNetworkInterface::interfaceFromName ( QString ( MULTICAST_INTERFACE_SSL_PACKET ) ) ) ;

    udpSocketSslPacket->bind ( portSslPacket, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint  );
    udpSocketSslPacket->joinMulticastGroup ( *netAddressSslPacket, *netInterfaceSslPacket );
    connect( udpSocketSslPacket, SIGNAL ( readyRead() ),  this, SLOT ( processPendingDatagramsSslPacket() ) );


    cmd = Stop;

    game = new Game ( BLUE, LEFT );
    enemyGame = new Game ( YELLOW, RIGHT );
}

GameMainWindow::~GameMainWindow()
{
    delete udpSocketReferee;
    delete udpSocketSslPacket;
    delete netAddressSslPacket;
    delete netInterfaceSslPacket;
}

void GameMainWindow::playGame(Game *game)
{
    //game->setCmdReferee ( Referee::Stop );
    qDebug() << "\nTEAM " << ( game->getTeamColor() == BLUE ? "BLUE" : "YELLOW" ) << "cmd" << (char)cmd << "\n";
    if (    cmd == Ready ||
            //cmd == KickOffYellow ||
            cmd == DirectFreeKickYellow ||
            cmd == IndirectFreeKickYellow ||
            //cmd == KickOffBlue ||
            cmd == DirectFreeKickBlue ||
            cmd == IndirectFreeKickBlue )
        //a bola entrou em jogo, portanto, muda o comando para o force start
        if ( game->getStrategy()->ballInPlay() )
        {
            cmd = Start;
            game->setReadyEnemy ( false );
            //exit ( 0 );
        }

    game->setCmdReferee ( cmd );
    game->beginStrategy();
    int lim = game->getRobots()->size();
//    qDebug() << "lim" << lim;
    for ( int i = 0; i < lim; i++ )
    {
        Robot *rob = game->getRobots()->at ( i );
//        qDebug() << rob->getId() << rob->getIsOn();
        if ( rob->getIsOn() )
        {
            rob->resetRobot();
        }
    }
    game->resetEnemyRobots();
}

void GameMainWindow::processPendingDatagramsReferee()
{
    while (udpSocketReferee->hasPendingDatagrams())
    {
        /**

          FOR THE NEW PROTOCOL USE THIS

        QByteArray datagram;
        mutexReferee.lock();
        datagram.resize(udpSocketReferee->pendingDatagramSize());
        udpSocketReferee->readDatagram(datagram.data(), datagram.size());
        mutexReferee.unlock();

        if (!referee.ParseFromArray(datagram.constData(), static_cast<int>(datagram.length())))
        {
            qDebug () << "Protobuf parsing error!\n";
            exit ( 0 );
        }
        cmd = referee.command();
        */

        QByteArray datagram;
        mutexReferee.lock();
        datagram.resize(udpSocketReferee->pendingDatagramSize());
        udpSocketReferee->readDatagram(datagram.data(), datagram.size());
        mutexReferee.unlock();

        cmd =  datagram.data() [ 0 ];


//        qDebug() << "TS=" << referee.packet_timestamp() << ", stage=" << referee.stage() << ", stage_time_left=" << referee.stage_time_left() << ", command=" << referee.command() << ", yscore=" << referee.yellow().score() << ", bscore=" << referee.blue().score() << '\n';

    }
}

void GameMainWindow::processPendingDatagramsSslPacket()
{
    if ( udpSocketSslPacket->state() == QUdpSocket::BoundState && udpSocketSslPacket->hasPendingDatagrams() )
    {
        QByteArray datagram;
        mutex.lock();
        datagram.resize(udpSocketSslPacket->pendingDatagramSize());
        udpSocketSslPacket->readDatagram(datagram.data(), datagram.size());
        mutex.unlock();
        //decode packet:
        SSL_WrapperPacket packet;
        packet.ParseFromArray(datagram.data(), datagram.size());

        if ( packet.has_detection() )
        {

            SSL_DetectionFrame detection = packet.detection();
            int balls_n = detection.balls_size();
            int robots_blue_n =  detection.robots_blue_size();
            int robots_yellow_n =  detection.robots_yellow_size();

            //Ball info:
            for (int i = 0; i < balls_n; i++)
            {
                SSL_DetectionBall ball = detection.balls(i);
                double bX = ball.x();
                double bY = ball.y();
                double bZ = 0;
                if ( ball.has_z() )
                {
                    bZ = ball.z ();
                }
                game->setDetectedBall ( bX, bY, bZ );
                enemyGame ->setDetectedBall ( bX, bY, bZ );
//                    if (    ball.x() < FIELD_HORIZONTAL_MIN || ball.x() > FIELD_HORIZONTAL_MAX ||
//                            ball.y() < FIELD_VERTICAL_MIN || ball.y() > FIELD_VERTICAL_MAX )
//                        cmd = Halt;
            }
            //Blue robot info:
            for (int i = 0; i < robots_blue_n; i++)
            {
                SSL_DetectionRobot robot = detection.robots_blue(i);
                if ( game->getTeamColor() == BLUE )
                {
                    game->setDetectedRobot ( robot.robot_id(), robot.x(), robot.y(), robot.orientation() );
                    enemyGame->setDetectedEnemyRobot ( robot.robot_id(), robot.x(), robot.y(), robot.orientation() );
                }
                else
                {
                    game->setDetectedEnemyRobot ( robot.robot_id(), robot.x(), robot.y(), robot.orientation() );
                    enemyGame->setDetectedRobot ( robot.robot_id(), robot.x(), robot.y(), robot.orientation() );
                }
            }

            //Yellow robot info:
            for (int i = 0; i < robots_yellow_n; i++)
            {
                SSL_DetectionRobot robot = detection.robots_yellow(i);
                if ( enemyGame->getTeamColor() == YELLOW )
                {
                    enemyGame->setDetectedRobot ( robot.robot_id(), robot.x(), robot.y(), robot.orientation() );
                    game->setDetectedEnemyRobot ( robot.robot_id(), robot.x(), robot.y(), robot.orientation() );
                }
                else
                {
                    enemyGame->setDetectedEnemyRobot ( robot.robot_id(), robot.x(), robot.y(), robot.orientation() );
                    game->setDetectedRobot ( robot.robot_id(), robot.x(), robot.y(), robot.orientation() );
                }
            }

        }

        playGame( game );
        playGame( enemyGame );
    }
}

void GameMainWindow::printRobotInfo(const SSL_DetectionRobot & robot)
{
    printf("CONF=%4.2f ", robot.confidence());
    if (robot.has_robot_id()) {
        printf("ID=%3d ",robot.robot_id());
    } else {
        printf("ID=N/A ");
    }
    printf(" HEIGHT=%6.2f POS=<%9.2f,%9.2f> ",robot.height(),robot.x(),robot.y());
    if (robot.has_orientation()) {
        printf("ANGLE=%6.3f ",robot.orientation());
    } else {
        printf("ANGLE=N/A    ");
    }
    printf("RAW=<%8.2f,%8.2f>\n",robot.pixel_x(),robot.pixel_y());
}

