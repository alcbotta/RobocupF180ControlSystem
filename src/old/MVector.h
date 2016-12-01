#ifndef MVECTOR_H
#define MVECTOR_H

#include <cmath>
#include "UtilGame.h"

class MVector
{
    public:
        MVector();
        MVector ( double nDirection, double tDirection );
        virtual ~MVector();
        void setNormalDirection ( double nDirection );
        virtual double getNormalDirection();
        void setTangentialDirection ( double tDirection );
        virtual double getTangentialDirection();
        double getModule ();
        virtual double getAngle ();
        virtual double getOppositeAngle ();
        void showInformation ();

    private:
        double normalDirection;
        double tangentialDirection;
};

#endif // MVECTOR_H
