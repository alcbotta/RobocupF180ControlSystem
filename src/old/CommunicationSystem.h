#ifndef COMMUNICATION_SYSTEM_H
#define COMMUNICATION_SYSTEM_H

#include "Robot.h"
class CommunicationSystem
{
public:
    CommunicationSystem ();
    virtual ~CommunicationSystem ();
    virtual void sendCommand ( Robot *robot );
};



#endif
