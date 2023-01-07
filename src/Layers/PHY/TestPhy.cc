#include <string.h>
#include <omnetpp.h>

#include "../Packets/nbPacket_m.h"
#include "Phy.h"


using namespace omnetpp;

class TestPhy : public cSimpleModule
{
    protected:
       virtual void initialize() override;
       virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TestPhy);



void TestPhy::initialize()
{
    nbPacket *newPacket = new nbPacket("testUpperPacket");
    newPacket->setType(RA_PREAMBLE);

    EV << "Sending ";
    EV << newPacket->getType() << endl;

    sendDelayed (newPacket,0.003,"out");

}



void TestPhy::handleMessage(cMessage *msg)
{
    nbPacket *packetPhy = check_and_cast<nbPacket *>(msg);



    if (packetPhy->getType() == SIB2) {
        EV << "Receiving type ";
        EV << "SIB2" << endl;
    }

    else if (packetPhy->getType() == RA_RESPONSE) {
        EV << "Receiving type ";
        EV << "RA_RESPONSE" << endl;

        packetPhy->setType(RA_FEEDBACK);
        send(packetPhy,"out");

        EV << "Sending : RA_FEEDBACK" << endl;

    }

    else if (packetPhy->getType() == RA_CONTENSION_RESOLUTION) {
        EV << "Receiving type ";
        EV << "RA_CONTENSION_RESOLUTION" << endl;


        nbPacket *newPacket = new nbPacket("testUpperPacket");
        newPacket->setType(MAC_SDU);


        send(newPacket,"out");
        EV << "Sending : MAC_SDU" << endl;
    }


}
