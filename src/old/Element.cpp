#include "Element.h"

Element::Element ()
{
    position = new Point ( 0, 0 );
    velocity = new MVector ( 0, 0 );
}

Element::Element ( Point *pos )
{
    position = pos;
    velocity = new MVector ( 0, 0 );
}

Element::Element ( Point *pos, MVector *vel )
{
    position = pos;
    velocity = vel;
}

Element::~Element (){ }

void Element::setPosition ( Point *pos )
{
    position = pos;
}

Point *Element::getPosition ()
{
    return position;
}

void Element::setVelocity ( MVector *vel )
{
    velocity = vel;
}

MVector *Element::getVelocity ()
{
    return velocity;
}
