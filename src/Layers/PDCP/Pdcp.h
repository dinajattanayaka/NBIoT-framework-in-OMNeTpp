
#ifndef LAYERS_PDCP_PDCP_H_
#define LAYERS_PDCP_PDCP_H_


#include <string.h>
#include <omnetpp.h>
#include "pdcpPacket_m.h"

using namespace omnetpp;

class Pdcp : public cSimpleModule
{
  protected:
    int lowerGateIn_;
    int lowerGateOut_;
    int upperGateIn_;
    int upperGateOut_;
    int state;
    cQueue upperQueue;
    cQueue lowerQueue;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
};

Define_Module(Pdcp);




#endif /* LAYERS_PDCP_PDCP_H_ */
