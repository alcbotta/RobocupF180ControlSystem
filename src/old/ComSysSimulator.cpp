#include "ComSysSimulator.h"

ComSysSimulator::ComSysSimulator()
{
    _port = 20011;
    _addr = "127.0.0.1";
}

void ComSysSimulator::sendCommand ( Robot *robot )
{
    grSim_Packet packet;
    packet.mutable_commands()->set_isteamyellow( robot->getColor() == YELLOW ? true : false );
    packet.mutable_commands()->set_timestamp(0.0);
    grSim_Robot_Command* command = packet.mutable_commands()->add_robot_commands();
    command->set_id( robot->getId() );

    command->set_wheelsspeed(false);
    command->set_wheel1(0);
    command->set_wheel2(0);
    command->set_wheel3(0);
    command->set_wheel4(0);

    command->set_veltangent(robot->getVelocity()->getTangentialDirection());//
    command->set_velnormal(robot->getVelocity()->getNormalDirection());
    command->set_velangular(robot->getAngularVelocity());
    command->set_kickspeedx(robot->getKick());
    command->set_kickspeedz(robot->getDribble());
    command->set_spinner(false);

    QByteArray dgram;
    dgram.resize(packet.ByteSize());
    packet.SerializeToArray(dgram.data(), dgram.size());
    udpsocket.writeDatagram(dgram, _addr, _port);
}
