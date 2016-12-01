#ifndef COM_SYS_SIMULATOR_H
#define COM_SYS_SIMULATOR_H

#include "CommunicationSystem.h"

#include <QMutex>
#include <QtNetwork>
#include "grSim_Packet.pb.h"
#include "grSim_Commands.pb.h"
#include "grSim_Replacement.pb.h"
#include <QString>
#include <string>

class ComSysSimulator : public CommunicationSystem
{
public:
    ComSysSimulator ();
    void sendCommand ( Robot *robot );

private:

    bool sending, reseting;
    QUdpSocket udpsocket;
    QHostAddress _addr;
    quint16 _port;
};

#endif
