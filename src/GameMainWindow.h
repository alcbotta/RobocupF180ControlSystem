#ifndef GAME_MAIN_WINDOW_H
#define GAME_MAIN_WINDOW_H

#include <QMainWindow>

#include <QHostAddress>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QMutex>

#include "referee.pb.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"
#include "messages_robocup_ssl_refbox_log.pb.h"

#include "grSim_Packet.pb.h"
#include "grSim_Commands.pb.h"
#include "grSim_Replacement.pb.h"

#include "./src/old/Game.h"



class GameMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameMainWindow(QMainWindow *parent = 0);
     ~GameMainWindow();


    void playGame ( Game *game );

signals:

public slots:


private slots:
    void processPendingDatagramsReferee();
    void processPendingDatagramsSslPacket();

private:
    QUdpSocket *udpSocketReferee;
    QHostAddress groupAddressReferee;
    SSL_Referee referee;
    QMutex mutexReferee;


    QUdpSocket * udpSocketSslPacket;
    QMutex mutex;
    quint16 portSslPacket;
    QHostAddress *netAddressSslPacket;
    QNetworkInterface *netInterfaceSslPacket;


    void printRobotInfo(const SSL_DetectionRobot &robot);

    Game *game;
    Game *enemyGame;
    char cmd;
};

#endif // GAME_H
