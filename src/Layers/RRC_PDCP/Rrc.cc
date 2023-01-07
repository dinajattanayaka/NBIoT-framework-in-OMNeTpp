#include <string.h>
#include <omnetpp.h>

#include "Rrc.h"

#include "../Packets/nbPacket_m.h"


using namespace omnetpp;

void Rrc::initialize()
{
    lowerGateIn_ = findGate("lowerGateIn");
    lowerGateOut_ = findGate("lowerGateOut");
    upperGateIn_ = findGate("upperGateIn");
    upperGateOut_ = findGate("upperGateOut");
    state = RRC_IDLE ; //Idle
}

void Rrc::handleMessage(cMessage *msg)
{
    nbPacket *packetRrc = check_and_cast<nbPacket *>(msg);


    if(packetRrc->getArrivalGate()->getId()==lowerGateIn_){


        if (packetRrc->getType() == CONNECTION_SETUP){

                EV <<"RRC_PDCP lowerGate receiving Type : CONNECTION_SETUP " <<endl;
                packetRrc->setType(CONNECTION_SETUP_COMPLETE);
                state = RRC_CONNECTED;
                EV <<"State is changed"<< endl;
                send(packetRrc, "lowerGateOut");
                EV << "RRC_PDCP lowerGate Sending type ";
                EV << "CONNECTION_SETUP_COMPLETE" << endl;

//                if (!lowerQueue.isEmpty()){
//                    nbPacket* newPacket = check_and_cast<nbPacket *>(lowerQueue.pop());
//                    newPacket->setType(RRC_SDU);
//                    send(newPacket,"lowerGateOut");
//                    EV << "RRC_PDCP lowerGate sending type: ";
//                    EV << "RRC_SDU" << endl;
//
//                }
//                else {
//                    EV<<"$$$$$$$$$$ ERROR $$$$$$$$$"<<endl;
//                }
                nbPacket* newPacket = check_and_cast<nbPacket *>(lowerQueue.pop());
                newPacket->setType(RRC_SDU);
                send(newPacket,"lowerGateOut");
                EV << "RRC_PDCP lowerGate sending type: ";
                EV << "RRC_SDU" << endl;


        }




        else if (packetRrc->getType() == CONNECTION_RELEASE){

            EV << "RRC_PDCP lowerGate receiving type : CONNECTION_RELEASE "<<endl;
            state = RRC_IDLE;
            EV<<"$$$$$$$$$$  Changing the state to : RRC_IDLE $$$$$$$$$"<<endl;

        }

        else if (packetRrc->getType() == RRC_PDU){
            EV <<"RRC_PDCP lowerGate receiving Type : RRC_PDU " <<endl;
            packetRrc->setType(NET_PDU);
            send(packetRrc, "upperGateOut");
            EV << "RRC_PDCP upperGate sending type ";
            EV << "NET_PDU" << endl;
        }
    }



    else if(packetRrc->getArrivalGate()->getId()==upperGateIn_){
        EV <<"RRC_PDCP upperGate receiving Type ";
        EV << packetRrc->getType()<< endl;


        lowerQueue.insert(packetRrc);

        if (state == RRC_CONNECTED){
            if (!lowerQueue.isEmpty()) {
                nbPacket* reply = check_and_cast<nbPacket*>(lowerQueue.pop());

                if (packetRrc->getType() == NET_SDU){
                    packetRrc->setType(RRC_SDU);

                    EV << "Sending real data" << endl;
                    send(reply, "lowerGateOut");

                    EV << "RRC_PDCP lowerGate sending type: ";
                    EV << "RRC_SDU" << endl;

                }
            }

            else {
                EV <<"RRC queue is empty"<<endl;
            }

//            send(packetRrc, "lowerGateOut");
//            EV << "lowerGate sending type ";
//            EV << "SDU" << endl;


        }
        else{
            nbPacket* newPacket = new nbPacket("newPacket");
            newPacket->setType(CONNECTION_SETUP_REQUEST);
            send(newPacket, "lowerGateOut");
            EV << "RRC_PDCP lowerGate sending type ";
            EV << "CONNECTION_SETUP_REQUEST" << endl;


        }
    }
}


