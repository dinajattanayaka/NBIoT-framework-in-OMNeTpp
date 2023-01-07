#include "../Packets/nbPacket_m.h"
#include "Rlc.h"


class TestRlc : public cSimpleModule
{
    protected:
       virtual void initialize() override;
       virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TestRlc);


void TestRlc::initialize()
{
    if (strcmp("TestRlc2", getName()) == 0) {  // TestRlc - RlcSDU/conSetupeqR/conSetupAck and TestRlc2 - rlcPDU/ conSetup
        nbPacket *newPacket = new nbPacket("testPacket");
        newPacket->setType(RLC_PDU);

        EV << "Sending ";
        EV << newPacket->getType() << endl;

        send (newPacket,"out");

    }
}

void TestRlc::handleMessage(cMessage *msg)
{
    nbPacket *msgRlc = check_and_cast<nbPacket *>(msg);

    if (strcmp("TestRlc", getName()) == 0) {
        EV << "type ";
        EV << msgRlc->getType() << endl;
    }
    else if (strcmp("TestRlc2", getName()) == 0) {
        EV << "type ";
        EV << msgRlc->getType() << endl;
    }
}




