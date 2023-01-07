#include <string.h>
#include <omnetpp.h>
#include <math.h>

#include "App.h"

#include "../Packets/nbPacket_m.h"


using namespace omnetpp;

void App::initialize()
{
    lowerGateIn_ = findGate("lowerGateIn");
    lowerGateOut_ = findGate("lowerGateOut");
    interTransmit = par("interTransmitTime");

    if (strcmp("App", getName()) == 0) {
           nbPacket *Appmsg = new nbPacket("DataMsg");

           uID = getParentModule()->getIndex();       //get the id of the Ue
           EV<<"User ID is "<<uID<<endl;

           srand(time(0)+uID);
//           randValue = (rand()%3000)*0.000001;
//           randValue = uniform(0,3600);
//           randValue = (rand()%(3600*interTransmit));

           randValue = fmod (rand(),(3600*interTransmit));


           EV<<"Random Value is :"<<randValue<<endl;

           Appmsg->setUeID(uID);
           Appmsg->setType(NET_SDU);
//           sendDelayed(Appmsg,0.005+randValue,"lowerGateOut");
           sendDelayed(Appmsg,randValue,"lowerGateOut");

           nbPacket *psMsg = new nbPacket("psMsg");
           psMsg->setType(NET_SDU);
           psMsg->setUeID(uID);
           scheduleAt(simTime() + 3600*interTransmit+randValue,psMsg);

           EV << "Sending Initial APP Message"<< endl;
    }

}

void App::handleMessage(cMessage *msg)
{
    nbPacket *packetApp = check_and_cast<nbPacket *>(msg);

    if (packetApp->isSelfMessage()){
        delete packetApp;
        nbPacket *newMsg = new nbPacket("testMsg");
        newMsg->setType(NET_SDU);
        newMsg->setUeID(uID);
        send(newMsg, "lowerGateOut");

        EV << "Sending New APP Message " << endl;

        nbPacket *psMsg = new nbPacket("psMsg");
        psMsg->setType(NET_SDU);
        psMsg->setUeID(uID);
        scheduleAt(simTime() + 3600*interTransmit,psMsg);
    }

//    if(packetApp->getArrivalGate()->getId()==lowerGateIn_){
//        EV <<"APP lowerGate receiving Type "<< endl;
//        EV << packetApp->getType() << endl;
//
//    }

}

