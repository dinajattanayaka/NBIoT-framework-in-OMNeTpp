
#ifndef LAYERS_APP_APP_H_
#define LAYERS_APP_APP_H_



#include <string.h>
#include <omnetpp.h>

#include "../Packets/nbPacket_m.h"

using namespace omnetpp;


class App : public cSimpleModule
{
  protected:
    int lowerGateIn_;
    int lowerGateOut_;

    double interTransmit;
    int uID;
    double randValue;


    cPacketQueue lowerQueue;


  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
};

Define_Module(App);



#endif /* LAYERS_APP_APP_H_ */
