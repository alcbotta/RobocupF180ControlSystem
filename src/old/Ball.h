#ifndef BALL_H
#define BALL_H

#include "Element.h"
//#include <sys/time.h>

#define NEURAL_TIME_MAX 5.0
#include <unistd.h>

class BallSettings
{
    public:
        double ballZ_Simulation;
        double ballVelocityMin;        
};

class Ball : public Element
{
    public:

        //construtor default para uma bola
        Ball ();
        //função sobrepõe a função da classe Element para que seja possível calcular a velocidade da bola
        void setPosition ( Point *pos );
//        Point *getBallPredicted ( double pTime = NEURAL_TIME_MAX );

        Point *getPosition ();
        BallSettings *ballSettings;
        void initBallSettings();
        bool isMoving ();

    private:
        double time1;
};

#endif
