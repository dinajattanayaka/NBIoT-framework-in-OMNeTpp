#include <string.h>
#include <omnetpp.h>


#include "../Packets/nbPacket_m.h"
#include "Mac.h"

using namespace omnetpp;


class TestMac : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TestMac);


void TestMac::initialize()
{
    if (strcmp("TestMac", getName()) == 0) {  // TestMAC - RLCSDU , CONNECTIONS_SETUP_REQUEST
        nbPacket *newPacket = new nbPacket("testUpperPacket");
        newPacket->setType(CONNECTION_SETUP_REQUEST);

        EV << "Sending ";
        EV << newPacket->getType() << endl;

        sendDelayed (newPacket,2000,"out");

    }
}

void TestMac::handleMessage(cMessage *msg)
{
    nbPacket *packetMac = check_and_cast<nbPacket *>(msg);

    if (packetMac->getType() == RLC_PDU) {
        EV << "Receiving type : ";
        EV << "RRC_PDU" << endl;
    }
    else if (packetMac->getType() == CONNECTION_SETUP){
        EV << "Receiving type : ";
        EV << "CONNECTION_SETUP" << endl;
    }
}


