#include "Game.h"
#include "CommunicationSystem.h"
#include "ComSysSimulator.h"

void Game::initFieldSettings()
{
    fieldSettings = new FieldSettings;

    fieldSettings->fieldHorizontalMin = -3025;
    fieldSettings->fieldHorizontalMax = 3025;
    fieldSettings->fieldVerticalMin = -2025;
    fieldSettings->fieldVerticalMax = 2025;
    fieldSettings->goalkeeperAreaRadius = 800;
    fieldSettings->goalkeeperPenalty = fieldSettings->fieldHorizontalMin + 65;
    fieldSettings->penaltyMark = 750;
    fieldSettings->penaltyGoalPoint = 200;//300;
    fieldSettings->goalWidth = 700;
    fieldSettings->robotDistancePenalty = fieldSettings->penaltyMark + 400;

//    cmdReferee = Stop;
//    lastCmdReferee = Stop;
}

Game::Game ( TeamColor mColor, FieldSide mSide ) :
    color ( mColor ), side ( mSide )
{
    initFieldSettings();
    strategy = new Strategy ( this );
    readyEnemy = false;

    enemyColor = color == BLUE ? YELLOW : BLUE;

    if ( side == LEFT )
    {
        goal = new Point ( -3025, 0 );
        enemyGoal = new Point ( 3025, 0 );
    }
    else
    {
        goal = new Point ( 3025, 0 );
        enemyGoal = new Point ( -3025, 0 );
    }

    Behavior initBehaviors [] = {   GOALKEAPER, DEFENDER_LEFT, DEFENDER_RIGHT, MIDFIELD_ONLY,
                                    STRIKER_LEFT, STRIKER_RIGHT };

    playersOn = 0;
    robots = new vector < Robot * >;

#ifdef REAL_GAME
    ComSysXBot *comSysXBot = new ComSysXBot ();

    robots->push_back ( new Robot ( 0, &color, 0, comSysXBot ) );
    robots->at ( 0 )->setRobotBehavior ( new RobotBehaviorGoalkeaper ( initBehaviors [ 0 ], robots->at ( 0 ), strategy ) );

    for ( int i = 1; i < ROBOT_COUNT; i++ )
    {
        robots->push_back ( new Robot ( i, &color, i, comSysXBot ) );
        robots->at ( i )->setRobotBehavior ( new RobotBehavior ( initBehaviors [ i ], robots->at ( i ), strategy ) );
    }
#else

    ComSysSimulator *comSysSimulator = new ComSysSimulator ();

    robots->push_back ( new Robot ( 0, &color, 0, comSysSimulator ) );
    robots->at ( 0 )->setRobotBehavior ( new RobotBehaviorGoalkeaper ( initBehaviors [ 0 ], robots->at ( 0 ), strategy ) );

    for ( int i = 1; i < ROBOT_COUNT; i++ )
    {
        robots->push_back ( new Robot ( i, &color, i, comSysSimulator ) );
        robots->at ( i )->setRobotBehavior ( new RobotBehavior ( initBehaviors [ i ], robots->at ( i ), strategy ) );
    }
#endif

    enemyRobots = new vector < Robot * >;
    for ( int i = 0; i < ROBOT_COUNT; i++ )
    {
        enemyRobots->push_back ( new Robot ( i, &enemyColor, i, NULL ) );
        enemyRobots->at ( i )->setRobotBehavior ( new RobotBehavior ( ENEMY, enemyRobots->at ( i ), strategy ) );
    }

    teamSize = robots->size();
    enemySize = enemyRobots->size();
    ball = new Ball ();


    initGoalPost ();
}

Game::~Game () {}

void Game::beginStrategy ()
{
    this->changePlayers();

    if ( this->getPlayersOn() != 0 )
        strategy->makeStrategy();
}

void Game::initializeGame()
{
//    strategy = new Strategy ( this );
//    initGoalPost ();
}

int Game::getPlayersOn ()
{
    return playersOn;
}

void Game::showRobotInformation(Robot *m_robot)
{
    m_robot->showInformation();
    cout << "distancia da bola = " << m_robot->getPosition()->getDistanceTo ( this->getBall()->getPosition() ) << endl;
    cout << endl;
}

void Game::initGoalPost()
{
    double goalY = fieldSettings->goalWidth / 2;
    double goalX = this->getOurGoal()->getPointX();
    changeSide ( goalY );
    goalPostUpper = new Point ( goalX, goalY );
    goalPostBottom = new Point ( goalX, -goalY );

    goalX = this->getEnemyGoal()->getPointX();
    goalY = fieldSettings->goalWidth / 2;
    changeSide ( goalY );
    enemyGoalPostUpper = new Point ( goalX, goalY );
    enemyGoalPostBottom = new Point ( goalX, -goalY );
}

void Game::changeSide ( double &valueToChange )
{
    if ( getSide() == RIGHT )
        valueToChange *= -1;
}

void Game::updateRobotBehavior ( Robot *robot, Behavior behavior )
{
    if ( robot->getBehavior() == behavior )
        return;

    if ( behavior == GOALKEAPER || behavior == GOALKEAPER_ONLY )
        robot->setRobotBehavior ( new RobotBehaviorGoalkeaper ( behavior, robot, strategy ) );
    else
        robot->setRobotBehavior ( new RobotBehavior ( behavior, robot, strategy ) );
}

Point *Game::getEnemyGoalPostUpper()
{
    return enemyGoalPostUpper;
}

Point *Game::getEnemyGoalPostBottom()
{
    return enemyGoalPostBottom;
}

Point *Game::getOutGoalPostUpper()
{
    return goalPostUpper;
}

Point *Game::getOutGoalPostBottom()
{
    return goalPostBottom;
}

void Game::setReadyEnemy(bool ready)
{
    readyEnemy = ready;
}

bool Game::getReadyEnemy()
{
    return readyEnemy;
}

vector<Robot *> *Game::getRobotsOn()
{
    vector<Robot *> *robotsOn = new vector<Robot *>;
    for ( int i = 0; i < robots->size(); i++ )
        if ( robots->at ( i )->getIsOn () )
            robotsOn->push_back( robots->at ( i ) );
    return robotsOn;
}

vector<Robot *> *Game::getEnemiesOn()
{
    vector<Robot *> *robotsOn = new vector<Robot *>;
    for ( int i = 0; i < enemyRobots->size(); i++ )
        if ( enemyRobots->at ( i )->getIsOn () )
            robotsOn->push_back( enemyRobots->at ( i ) );
    return robotsOn;
}

Strategy *Game::getStrategy ()
{
    return strategy;
}

char Game::getCmdReferee ()
{
    return cmdReferee;
}

char Game::getLastCmdReferee ()
{
    return lastCmdReferee;
}

//seta o comando enviado pelo juiz
void Game::setCmdReferee ( char newCmdReferee )
{
    if ( cmdReferee != newCmdReferee )
        lastCmdReferee = cmdReferee;
    cmdReferee = newCmdReferee;
}

vector < Robot * > *Game::getRobots ()
{
    return robots;
}

vector < Robot * > *Game::getEnemyRobots ()
{
    return this->getEnemiesOn();
    //return enemyRobots;
}

TeamColor Game::getTeamColor ()
{
    return color;
}

Point *Game::getEnemyGoal ()
{
    return enemyGoal;
}

Point *Game::getOurGoal ()
{
    return goal;
}

//seta as informações de cada robô detectado pela câmera
void Game::setDetectedRobot ( int robotId, double posX, double posY, double newOrientation )
{
    robots->at ( robotId )->setIsOn ( true );
    robots->at ( robotId )->setPosition ( new Point ( posX, posY ) );
    robots->at ( robotId )->setOrientation ( newOrientation );
}

void Game::setDetectedEnemyRobot ( int robotId, double posX, double posY, double newOrientation )
{
    enemyRobots->at ( robotId )->setIsOn ( true );
    enemyRobots->at ( robotId )->setPosition ( new Point ( posX, posY ) );
    enemyRobots->at ( robotId )->setOrientation ( newOrientation );
}

void Game::resetEnemyRobots ()
{
    int lim = enemyRobots->size();
    for ( int i = 0; i < lim; i++ )
        enemyRobots->at ( i )->resetRobot ();
}

//seta as informações da bola detectada pela câmera
void Game::setDetectedBall ( double posX, double posY, double posZ )
{
    //cout << "posX = " << posX << "\tposY = " << posY << "\tposZ " << posZ << endl;
    ball->setPosition ( new Point ( posX, posY, posZ ) );
    //ball->getPosition()->toString();
    //automaticReferee();
}



Ball *Game::getBall ()
{
    return ball;
}

//retorna o lado do nosso campo
FieldSide Game::getSide ()
{
    return side;
}

//altera o comportamento de cada robo com base na quantidade de robos ativados
void Game::changePlayers ()
{
    playersOn = 0;
    vector < int > *actives = new vector <int>;

    for ( int i = 0; i < teamSize; i++ )
    {
        if ( robots->at ( i )->getIsOn() )
        {
            playersOn++;
            actives->push_back ( i );

        }
        else
        {
            //robots->at ( i )->setBehavior ( OFF );
            this->updateRobotBehavior ( robots->at ( i ), GOALKEAPER );
        }
    }

    if ( playersOn == ROBOT_COUNT )
    {
        this->updateRobotBehavior ( robots->at ( actives->at ( 0 ) ), GOALKEAPER );
        this->updateRobotBehavior ( robots->at ( actives->at ( 1 ) ), DEFENDER_LEFT );
        this->updateRobotBehavior ( robots->at ( actives->at ( 2 ) ), DEFENDER_RIGHT );
        this->updateRobotBehavior ( robots->at ( actives->at ( 3 ) ), MIDFIELD_ONLY );
        this->updateRobotBehavior ( robots->at ( actives->at ( 4 ) ), STRIKER_LEFT );
        this->updateRobotBehavior ( robots->at ( actives->at ( 5 ) ), STRIKER_RIGHT );
    }
    else if ( playersOn == 5 )
    {
        this->updateRobotBehavior ( robots->at ( actives->at ( 0 ) ), GOALKEAPER );
        this->updateRobotBehavior ( robots->at ( actives->at ( 1 ) ), DEFENDER_LEFT );
        this->updateRobotBehavior ( robots->at ( actives->at ( 2 ) ), DEFENDER_RIGHT );
        this->updateRobotBehavior ( robots->at ( actives->at ( 3 ) ), MIDFIELD_ONLY );
        this->updateRobotBehavior ( robots->at ( actives->at ( 4 ) ), STRIKER_ONLY );
    }
    else if ( playersOn == 4 )
    {
        this->updateRobotBehavior ( robots->at ( actives->at ( 0 ) ), GOALKEAPER );
        this->updateRobotBehavior ( robots->at ( actives->at ( 1 ) ), DEFENDER_ONLY );
        this->updateRobotBehavior ( robots->at ( actives->at ( 2 ) ), MIDFIELD_LEFT );
        this->updateRobotBehavior ( robots->at ( actives->at ( 3 ) ), MIDFIELD_RIGHT );
    }
    else if ( playersOn == 3 )
    {
        this->updateRobotBehavior ( robots->at ( actives->at ( 0 ) ), GOALKEAPER );
        this->updateRobotBehavior ( robots->at ( actives->at ( 1 ) ), MIDFIELD_LEFT );
        this->updateRobotBehavior ( robots->at ( actives->at ( 2 ) ), MIDFIELD_RIGHT );
    }
    else if ( playersOn == 2 )
    {
        this->updateRobotBehavior ( robots->at ( actives->at ( 0 ) ), GOALKEAPER );
        this->updateRobotBehavior ( robots->at ( actives->at ( 1 ) ), DEFENDER_ONLY );
    }
    else if ( playersOn == 1 )
    {
        this->updateRobotBehavior ( robots->at ( actives->at ( 0 ) ), GOALKEAPER_ONLY );
    }
    else
        cout << "Que Aulus nos ajude pois não temos robôs\n\n\n\n" << endl;
}



void Game::showTeam ()
{
    for ( int i = 0; i < teamSize; i++ )
    {
        cout    << "\nid = " << robots->at ( i )->getId()
                << "\nbehavior = " << robots->at ( i )->getBehaviorStr()
                << "\nposX = " << robots->at ( i )->getPosition()->getPointX()
                << "\nposY = " << robots->at ( i )->getPosition()->getPointY()
                << "distancia para a bola = " << robots->at ( i )->getPosition()->getDistanceTo ( this->ball->getPosition() );
    }
}





//void Game::automaticReferee ()
//{
//    cout << "automaticReferee\n\n";
//    double x = ball->getPosition()->getPointX();
//    double y = ball->getPosition()->getPointY();

//    if (    x < fieldSettings->fieldHorizontalMin || x > fieldSettings->fieldHorizontalMax ||
//            y < fieldSettings->fieldVerticalMin || y > fieldSettings->fieldVerticalMax )
//        setCmdReferee( Referee::Halt );
//    cout << "this->getCmdReferee() = " << this->getCmdReferee() << endl;
//}


//funções extern para uso na biblioteca dinânmica
extern "C"
{
	Game *getGame ( TeamColor color, FieldSide side )
	{
	    return new Game ( color, side );
	}

	void deleteGame ( Game *game )
	{
        delete game;
    }
}


