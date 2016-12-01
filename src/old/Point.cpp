#include "Point.h"

Point::Point ()
{
    pointX = pointY = pointZ = 0;
}

Point::Point ( double pX, double pY, double pZ ) : pointX ( pX ), pointY ( pY ), pointZ ( pZ )
{ }

Point::~Point ()
{

}

void Point::setPointX ( double pX )
{
    pointX = pX;
}

double Point::getPointX ()
{
    return pointX;
}

void Point::setPointY ( double pY )
{
    pointY = pY;
}

double Point::getPointY ()
{
    return pointY;
}

void Point::setPointZ ( double pZ )
{
    pointZ = pZ;
}

double Point::getPointZ ()
{
    return pointZ;
}

//retorna a distância em módulo entre este ponto e o ponto "anotherPoint"
double Point::getDistanceTo ( Point *anotherPoint )
{
    double valueX = anotherPoint->getPointX() - this->getPointX();
    double valueY = anotherPoint->getPointY() - this->getPointY();
    return sqrt ( valueX * valueX + valueY * valueY );
}

//angulo em radianos
double Point::calculateAngleTo ( Point *anotherPoint )
{
    double x = anotherPoint->getPointX() - this->getPointX();
    double y = anotherPoint->getPointY() - this->getPointY();
    return atan2( y , x );
}

void Point::showInformation ()
{
    cout << "x = " << pointX << "\ty = " << pointY << "\tz = " << pointZ << endl;
}

