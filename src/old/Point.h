#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <iostream>
using namespace std;

class Point
{
    public:
        Point ();
        Point ( double pX, double pY, double pZ = 0 );
        virtual ~Point ();
        void setPointX ( double pX );
        double getPointX ();
        void setPointY ( double pY );
        double getPointY ();
        void setPointZ ( double pZ );
        double getPointZ ();
        double getDistanceTo ( Point *anotherPoint );
        double calculateAngleTo ( Point *anotherPoint );
        void showInformation ();

    private:
        double pointX;
        double pointY;
        double pointZ;
};

#endif
