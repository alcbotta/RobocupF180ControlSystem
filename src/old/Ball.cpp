#include "Ball.h"

void Ball::initBallSettings()
{
    ballSettings = new BallSettings;
    ballSettings->ballZ_Simulation = 21.5;//valor fixo da posição em z no simulador, quando a bola está no chão
    ballSettings->ballVelocityMin = 200;//em mm/s
}

bool Ball::isMoving()
{
    return ( this->getVelocity()->getModule() > ballSettings->ballVelocityMin );
}

Ball::Ball ()
{
//    LoadDynamicLibrary *loadLib = new LoadDynamicLibrary ;
//    backPropag = loadLib->loadLibBackPropagationBall();

    time1 = UtilGame::countTime ();
    initBallSettings();
}

//sobreescreve o método da classe Element pois calcula a posição bola pela rede neural
void Ball::setPosition ( Point *pos )
{
    double time2 = UtilGame::countTime ();
    double dt = time2 - time1;

    double dr = position->getDistanceTo ( pos );
    this->setVelocity ( new MVector ( 0, 0 ) );
    if ( dt != 0 && dr != 0 )
    {
        double velocity = dr / dt;
        double angle = position->calculateAngleTo ( pos );
        this->setVelocity ( new MVector ( velocity * cos ( angle ), velocity * sin ( angle ) ) );

//        int max = 300;
//        if ( velocity > 200 )
//        {
//            this->setVelocity ( new MVector ( velocity * cos ( angle ), velocity * sin ( angle ) ) );
//            //está adicionando o Z da bola neste instante de tempo, e não no futuro, como a estratégia está baseada apenas se
//            //o valor de Z é diferente de uma valor definido, isto não causara problema, mas se depender do valor de X aí tem que alterar !!!!
//            nextPos = new Point ( realPosition->getPointX() + max * cos ( angle ), realPosition->getPointY() + max * sin ( angle ), pos->getPointZ() );
//        }
    }
    else
        this->setVelocity( new MVector ( 0, 0 ) );

    //cout << "Ball::setPosition : velocity = " << this->getVelocity()->getModule() << endl;
    time1 = time2;
    position = pos;
}

/*
Point *Ball::getBallPredicted(double pTime)
{
    float ball_velocity = this->getVelocity()->getModule();
    if ( ball_velocity < ballSettings->ballVelocityMin )
        return Element::getPosition();
    Point *nextPos = position;
    float neural_pos = 0.0;

    neural_pos = backPropag->get_output( pTime, this->getVelocity()->getModule() );
    double angle = this->getVelocity()->getAngle();
    //cout << "Ball::getBallPredicted  velocity = " << this->getVelocity()->getModule() << endl;
//    cout << "Ball::getBallPredicted : neural_pos = " << neural_pos << endl;
    nextPos = new Point ( position->getPointX() + neural_pos * cos ( angle ), position->getPointY() + neural_pos * sin ( angle ), position->getPointZ() );
    return nextPos;
}*/

Point *Ball::getPosition()
{
    //return this->getBallPredicted();
    return Element::getPosition();
}
