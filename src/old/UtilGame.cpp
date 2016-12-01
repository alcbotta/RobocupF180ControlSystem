#include "UtilGame.h"

double UtilGame::adjustAngle(double &angle)
{
    if ( angle > PI )
        angle -= 2 * PI;
    else if ( angle < -PI )
        angle += 2 * PI;
}

double UtilGame::countTime()
{
    struct timeval tv;
    gettimeofday(&tv,0);
    return tv.tv_sec + tv.tv_usec/1e6;
}

double UtilGame::m_map ( double x, double in_min, double in_max, double out_min, double out_max )
{
    return ( x - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) + out_min;
}
