
#ifndef LAYERS_APP_TESTAPP_CC_
#define LAYERS_APP_TESTAPP_CC_

#include <string.h>
#include <omnetpp.h>
#include "App.h"


class TestApp : public cSimpleModule
{
    protected:
      // virtual void initialize() override;
       virtual void handleMessage(cMessage *msg) override;
};

Define_Module(TestApp);


//void TestApp::initialize()
//{
//    if (strcmp("TestApp", getName()) == 0) {  // TestApp - testSDU and TestApp2 - rrcPDU/ conSetup
//        nbPacket *newPacket = new nbPacket("testPacket");
//        newPacket->setType(NET_SDU);
//
//        EV << "Sending : ";
//        EV << newPacket->getType() << endl;
//
//        send(newPacket,"out");
//
//    }
//}


void TestApp::handleMessage(cMessage *msg)
{
    nbPacket *msgApp = check_and_cast<nbPacket *>(msg);

    delete msgApp;

    EV << "Deleted Arrived Message " << endl;

//    if (strcmp("TestApp", getName()) == 0) {
//        EV << "Receiving type : ";
//        EV << msgApp->getType() << endl;
//    }
}



#endif /* LAYERS_APP_TESTAPP_CC_ */
