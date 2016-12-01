#include "MVector.h"

MVector::MVector() {}

MVector::MVector ( double tDirection, double nDirection ):
    normalDirection ( nDirection ), tangentialDirection ( tDirection )
{ }

MVector::~MVector()
{

}

void MVector::setNormalDirection ( double nDirection )
{
    normalDirection = nDirection;
}

double MVector::getNormalDirection()
{
    return normalDirection;
}

void MVector::setTangentialDirection ( double tDirection )
{
    tangentialDirection = tDirection;
}

double MVector::getTangentialDirection()
{
    return tangentialDirection;
}

//retorna a magnitude deste vetor
double MVector::getModule ()
{
    return sqrt ( normalDirection * normalDirection + tangentialDirection * tangentialDirection );
}

//retorna o angulo do vetor
double MVector::getAngle ()
{
    return atan2 ( normalDirection , tangentialDirection );
}

double MVector::getOppositeAngle()
{
    double oppAngle = this->getAngle();
    oppAngle += PI;
    if ( oppAngle > PI )
        oppAngle -= 2 * PI;
    return oppAngle;
}

void MVector::showInformation()
{
    cout << "module = " << this->getModule() << "\torientation = " << this->getAngle() <<
            "\tnormalDirection = " << normalDirection << "\ttangentialDirection = " << tangentialDirection << endl;
}
