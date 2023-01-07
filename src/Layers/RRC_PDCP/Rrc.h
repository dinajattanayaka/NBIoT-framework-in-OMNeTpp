
#ifndef LAYERS_RRC_PDCP_RRC_H_
#define LAYERS_RRC_PDCP_RRC_H_


#include <string.h>
#include <omnetpp.h>

#include "../Packets/nbPacket_m.h"

using namespace omnetpp;


class Rrc : public cSimpleModule
{
  protected:
    int lowerGateIn_;
    int lowerGateOut_;
    int upperGateIn_;
    int upperGateOut_;
    int state;
    cPacketQueue upperQueue;
    cPacketQueue lowerQueue;



  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
};

Define_Module(Rrc);




#endif /* LAYERS_RRC_PDCP_RRC_H_ */
