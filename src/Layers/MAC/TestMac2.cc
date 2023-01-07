#include <string.h>
#include <omnetpp.h>


#include "../Packets/nbPacket_m.h"
#include "Mac.h"

using namespace omnetpp;


class TestMac2 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    int sendStrength;
    cPacketQueue testQueue;
    int counter;
    int fbcounter;
};

Define_Module(TestMac2);


void TestMac2::initialize()
{
    if (strcmp("TestMac2", getName()) == 0) {  // TestRrc - testSDU and TestRrc2 - rrcPDU/ conSetup
        nbPacket *newPacket = new nbPacket("testLowerPacket");
        newPacket->setType(SIB2);
        sendStrength = -75;
        counter = 0;
        fbcounter = 0;

        if (newPacket->getType() == SIB2){

            newPacket->setSignalStrength(sendStrength);
        }

        EV << "Sending ";
        EV << newPacket->getType() << endl;

        send (newPacket,"out");



        nbPacket *releasePacket = new nbPacket("releasePacket");
        releasePacket->setType(CONNECTION_RELEASE);

//        nbPacket *copyPacket=releasePacket->dup();
        scheduleAt(simTime() + 500, releasePacket);

//        EV << "Sending ";
//        EV << releasePacket->getType() << endl;
//
//        sendDelayed (releasePacket,500,"out");

    }
}


void TestMac2::handleMessage(cMessage *msg)
{
    nbPacket *packetMac = check_and_cast<nbPacket *>(msg);

    if (packetMac->isSelfMessage()){
        if (packetMac->getType() == CONNECTION_RELEASE) {

            nbPacket *copyPacket=packetMac->dup();

            send (packetMac,"out");
            scheduleAt(simTime() + 3600, copyPacket);

            EV << "Test sending : CONNECTION_RELEASE" << endl;
        }
    }


    else if (packetMac->getType() == MAC_SDU) {
        EV << "Test receiving type ";
        EV << "MAC_SDU" << endl;


    }

    else if (packetMac->getType() == RA_PREAMBLE){
        EV << "Test receiving type ";
        EV << "RA_PREAMBLE" << endl;

        if (counter ==0){
            testQueue.insert(packetMac);
            nbPacket *sendPacket = new nbPacket("sendPacket");
            sendPacket->setType(RA_RESPONSE);
            sendDelayed(sendPacket,0.004,"out");       ///send response after 5 preambles
            counter = 1;    /// for testing : counter++
            fbcounter = 0;
        }
        else {
            counter++;
            delete packetMac;     ///test this

//            if (sendStrength>= NORMAL_COVERAGE){
//                if (counter >=7){
//                    counter =0;
//                }
//            }
//            else if (NORMAL_COVERAGE> sendStrength && sendStrength>= ROBUST_COVERAGE){
//                if (counter >=31){
//                    counter =0;
//                }
//            }
//            else if (ROBUST_COVERAGE> sendStrength && sendStrength>= EXTREME_COVERAGE){
//                if (counter >=127){
//                    counter =0;
//                }
//            }

        }
    }

    else if (packetMac->getType() == RA_FEEDBACK){
        EV << "Test receiving type ";
        EV << "RA_FEEDBACK" << endl;

        if (fbcounter ==0){
            testQueue.insert(packetMac);
            nbPacket *sendPacket = new nbPacket("sendPacket");
            sendPacket->setType(RA_CONTENSION_RESOLUTION);
            sendDelayed(sendPacket,0.004,"out");
            fbcounter = 1;   //for testing : counter++
            counter = 0;
        }
        else {
            fbcounter++;
            delete packetMac;     ///test this

//            if (sendStrength>= NORMAL_COVERAGE){
//                if (fbcounter >=8){
//                    fbcounter =0;
//                }
//            }
//            else if (NORMAL_COVERAGE> sendStrength && sendStrength>= ROBUST_COVERAGE){
//                if (fbcounter >=32){
//                    fbcounter =0;
//                }
//            }
//            else if (ROBUST_COVERAGE> sendStrength && sendStrength>= EXTREME_COVERAGE){
//                if (fbcounter >=128){
//                    fbcounter =0;
//                }
//            }

        }
    }
}





