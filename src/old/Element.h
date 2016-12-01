#ifndef ELEMENT_H
#define ELEMENT_H

#include "MVector.h"
#include "Point.h"
#include "UtilGame.h"

#include <iostream>
using namespace std;

class Element
{
    public:
        Element ();
        Element ( Point *pos );
        Element ( Point *pos, MVector *vel );
        virtual ~Element ();
        virtual void setPosition ( Point *pos );
        virtual Point *getPosition ();
        virtual void setVelocity ( MVector *vel );
        virtual MVector *getVelocity ();

    protected:
        Point *position;
        MVector *velocity;
};

#endif
