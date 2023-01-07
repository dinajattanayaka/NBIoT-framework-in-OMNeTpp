#include <string.h>
#include <omnetpp.h>
#include "pdcpPacket_m.h"
#include "Pdcp.h"


class TestPdcp : public cSimpleModule
{
    protected:
        int trigger;
    protected:
       virtual void initialize() override;
       virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TestPdcp);



void TestPdcp::initialize()
{
    if (strcmp("TestPdcp", getName()) == 0) {               //TestPdcp - testSDU/conRequest/conSetupAck and TestPdpc2 - testPDU
        pdcpPacket *msg = new pdcpPacket("testMsg");
        msg->setType("testSDU");

        EV << "Generating  ";
        EV << msg->getType() << endl;


        if (strcmp("testPDU", msg->getType()) == 0){
            pdcpPacket* testPacket = new pdcpPacket("encapMsg");
            testPacket->setType("pdcpPDU");
            testPacket->encapsulate(check_and_cast<cPacket*>(msg));

            send(testPacket, "out");
            EV << "Sending ";
            EV << testPacket->getType() << endl;

        }
        else {

            pdcpPacket* testPacket = new pdcpPacket("encapMsg");
            testPacket->setType("rrcSDU");
            testPacket->encapsulate(check_and_cast<cPacket*>(msg));

            send(testPacket, "out");
            EV << "Sending ";
            EV << testPacket->getType() << endl;
        }
    }
}


void TestPdcp::handleMessage(cMessage *msg)
{
    pdcpPacket *msgPdcp = check_and_cast<pdcpPacket *>(msg);

    if (strcmp("TestPdcp", getName()) == 0) {
        if (strcmp("rrcPDU", msgPdcp->getType()) == 0){
                EV << "type ";
                EV << msgPdcp->getType() << endl;
        }
    }
    else if (strcmp("TestPdcp2", getName()) == 0) {
        if (strcmp("pdpcSDU", msgPdcp->getType()) == 0){
                EV << "type ";
                EV << msgPdcp->getType() << endl;
        }
    }
}




//Real simulation

/*
void TestPdcp::initialize()
{
    if (strcmp("TestPdcp2", getName()) == 0) {
        pdcpPacket *msg = new pdcpPacket("testMsg");
        msg->setType("pdcpPDU");

        EV << "Generating  ";
        EV << msg->getType() << endl;


//        pdcpPacket* testPacket = new pdcpPacket("rrcSDU");
//        testPacket->encapsulate(check_and_cast<cPacket*>(msg));
//
//        if (strcmp("testPDU", msg->getType()) == 0){
//            pdcpPacket* testPacket = new pdcpPacket("pdcpPDU");
//            testPacket->setType("pdcpPDU");
//            testPacket->encapsulate(check_and_cast<cPacket*>(msg));
//
//            send(testPacket, "out");
//            EV << "Sending ";
//            EV << testPacket->getType() << endl;
//
//        }
//        else {
//            send(msg, "out");
//            EV << "Sending ";
//            EV << msg->getType() << endl;
//        }


        send(msg, "out");
        EV << "Sending ";
        EV << msg->getType() << endl;

    }
}

void TestPdcp::handleMessage(cMessage *msg)
{
    pdcpPacket *msgPdcp = check_and_cast<pdcpPacket *>(msg);

    if (strcmp("TestPdcp", getName()) == 0) {
        if (strcmp("rrcPDU", msgPdcp->getType()) == 0){
                EV << "type ";
                EV << msgPdcp->getType() << endl;
        }

    }
    else if (strcmp("TestPdcp2", getName()) == 0) {
        if (strcmp("pdcpSDU", msgPdcp->getType()) == 0){
                EV << "type ";
                EV << msgPdcp->getType() << endl;
        }
    }

}
*/


