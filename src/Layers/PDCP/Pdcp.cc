#include <string.h>
#include <omnetpp.h>
#include "Pdcp.h"


using namespace omnetpp;

void Pdcp::initialize()
{
    lowerGateIn_ = findGate("lowerGateIn");
    lowerGateOut_ = findGate("lowerGateOut");
    upperGateIn_ = findGate("upperGateIn");
    upperGateOut_ = findGate("upperGateOut");
    state = 1 ;
}



//Real simulation

void Pdcp::handleMessage(cMessage *msg)
{
    pdcpPacket *msgPdcp = check_and_cast<pdcpPacket *>(msg);

    if(msgPdcp->getArrivalGate()->getId()==lowerGateIn_){
        if (strcmp("pdcpPDU", msgPdcp->getType()) == 0){
            EV << "lowerGate receiving type: ";
            EV << msgPdcp->getType() << endl;


            pdcpPacket* decapPacket = check_and_cast<pdcpPacket*>(msgPdcp->decapsulate());
            delete msgPdcp;
            decapPacket->setType("rrcPDU");
            pdcpPacket* newPacket = new pdcpPacket("encapMsg");
            newPacket->setType("rrcPDU");
            newPacket->encapsulate(check_and_cast<cPacket*>(decapPacket));
            upperQueue.insert(newPacket);

            if (state == 1){
                if (!upperQueue.isEmpty()) {
                    pdcpPacket* reply = check_and_cast<pdcpPacket*>(upperQueue.pop());
                    send(reply, "upperGateOut");

                    EV << "upperGate sending type: ";
                    EV << msgPdcp->getType() << endl;
                }

            }
        }
    }



    else if(msgPdcp->getArrivalGate()->getId()==upperGateIn_){
        if (strcmp("rrcSDU", msgPdcp->getType()) == 0){
            EV << "upperGate receiving type: ";
            EV << msgPdcp->getType() << endl;

            pdcpPacket* newPacket = new pdcpPacket("pdcpSDU");
            newPacket->setType("pdcpSDU");
            newPacket->encapsulate(check_and_cast<cPacket*>(msgPdcp));
            lowerQueue.insert(newPacket);

            if (state == 1){
                if (!lowerQueue.isEmpty()) {
                    pdcpPacket* reply = check_and_cast<pdcpPacket*>(lowerQueue.pop());
                    send(reply, "lowerGateOut");

                    EV << "lowerGate sending type: ";
                    EV << reply->getType() << endl;
                }

            }

        }
    }
}





/*
//For testing
void Pdcp::handleMessage(cMessage *msg)
{
    pdcpPacket *msgPdcp = check_and_cast<pdcpPacket *>(msg);

    if(msgPdcp->getArrivalGate()->getId()==lowerGateIn_){

        if (strcmp("pdcpPDU", msgPdcp->getType()) == 0){
            EV << "lowerGate receiving type: ";
            EV << msgPdcp->getType() << endl;


            pdcpPacket* newPacket = new pdcpPacket("decapMsg");
            delete msgPdcp;
            newPacket->setType("rrcPDU");
            send(newPacket, "upperGateOut");

            EV << "upperGate sending type: ";
            EV << msgPdcp->getType() << endl;
        }
        else if (strcmp("conSetup", msgPdcp->getType()) == 0){
            EV << "lowerGate receiving type: ";
            EV << msgPdcp->getType() << endl;


            pdcpPacket* newPacket = new pdcpPacket("decapMsg");
            delete msgPdcp;
            newPacket->setType("conSetup");
            send(newPacket, "upperGateOut");

            EV << "upperGate sending type: ";
            EV << msgPdcp->getType() << endl;
        }

    }



    else if(msgPdcp->getArrivalGate()->getId()==upperGateIn_){

            EV << "upperGate receiving type: ";
            EV << msgPdcp->getType() << endl;

            pdcpPacket* newPacket = new pdcpPacket("pdcpSDU");
            newPacket->setType("pdcpSDU");
            newPacket->encapsulate(check_and_cast<cPacket*>(msgPdcp));
            lowerQueue.insert(newPacket);

            if (state == 1){
                if (!lowerQueue.isEmpty()) {
                    pdcpPacket* reply = check_and_cast<pdcpPacket*>(lowerQueue.pop());
                    send(reply, "lowerGateOut");

                    EV << "lowerGate sending type: ";
                    EV << reply->getType() << endl;
                }

            }


    }
}
*/
















