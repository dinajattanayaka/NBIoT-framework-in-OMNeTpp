#include "../Packets/nbPacket_m.h"
#include "Rrc.h"


class TestRrc : public cSimpleModule
{
    protected:
       virtual void initialize() override;
       virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TestRrc);


void TestRrc::initialize()
{
    if (strcmp("TestRrc", getName()) == 0) {  // TestRrc - testSDU and TestRrc2 - rrcPDU/ conSetup
        nbPacket *newPacket = new nbPacket("testPacket");
        newPacket->setType(NET_SDU);

        EV << "Sending ";
        EV << newPacket->getType() << endl;

        send (newPacket,"out");

    }
}


void TestRrc::handleMessage(cMessage *msg)
{
    nbPacket *msgRrc = check_and_cast<nbPacket *>(msg);

    if (strcmp("TestRrc", getName()) == 0) {
        EV << "type ";
        EV << msgRrc->getType() << endl;
    }
    else if (strcmp("TestRrc2", getName()) == 0) {
        EV << "type ";
        EV << msgRrc->getType() << endl;
    }
}


