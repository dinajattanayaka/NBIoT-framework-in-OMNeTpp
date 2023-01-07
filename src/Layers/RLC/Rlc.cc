
#include <string.h>
#include <omnetpp.h>

#include "Rlc.h"

#include "../Packets/nbPacket_m.h"


using namespace omnetpp;

void Rlc::initialize()
{
    lowerGateIn_ = findGate("lowerGateIn");
    lowerGateOut_ = findGate("lowerGateOut");
    upperGateIn_ = findGate("upperGateIn");
    upperGateOut_ = findGate("upperGateOut");
}

void Rlc::handleMessage(cMessage *msg)
{
    nbPacket *packetRlc = check_and_cast<nbPacket *>(msg);

    if(packetRlc->getArrivalGate()->getId()==lowerGateIn_){
        EV <<"RLC lowerGate receiving Type ";
        EV << packetRlc->getType() <<endl;

        if (packetRlc->getType() == CONNECTION_SETUP){
                send(packetRlc, "upperGateOut");
                EV << "RLC upperGate Sending type ";
                EV << "CONNECTION_SETUP" << endl;
        }
        else if (packetRlc->getType() == RLC_PDU){
            packetRlc->setType(RRC_PDU);
            send(packetRlc, "upperGateOut");
            EV << "RLC upperGate sending type ";
            EV << "RRC_PDU" << endl;
        }

        else if (packetRlc->getType() == CONNECTION_RELEASE){

            send(packetRlc, "upperGateOut");
            EV << "RLC upperGate sending type ";
            EV << "CONNECTION_RELEASE" << endl;
        }
    }


    else if(packetRlc->getArrivalGate()->getId()==upperGateIn_){
        EV <<"RLC upperGate receiving Type ";
        EV << packetRlc->getType()<< endl;

        lowerQueue.insert(packetRlc);


        if (!lowerQueue.isEmpty()) {
            nbPacket* reply = check_and_cast<nbPacket*>(lowerQueue.pop());

            if (packetRlc->getType() == RRC_SDU){
                packetRlc->setType(RLC_SDU);

                send(reply, "lowerGateOut");
                EV << "RLC lowerGate sending type: ";
                EV << "RLC_SDU" << endl;

            }
            else if (packetRlc->getType() == CONNECTION_SETUP_REQUEST){
                send(reply, "lowerGateOut");
                EV << "RLC lowerGate sending type: ";
                EV << "CONNECTION_SETUP_REQUEST" << endl;

            }
            else if (packetRlc->getType() == CONNECTION_SETUP_COMPLETE){
                send(reply, "lowerGateOut");
                EV << "RLC lowerGate sending type: ";
                EV << "CONNECTION_SETUP_COMPLETE" << endl;

            }
        }
    }
}


