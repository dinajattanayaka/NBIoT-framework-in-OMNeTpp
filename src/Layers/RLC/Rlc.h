/*
 * Rlc.h
 *
 *  Created on: Sep 19, 2018
 *      Author: dinaj
 */

#ifndef LAYERS_RLC_RLC_H_
#define LAYERS_RLC_RLC_H_


#include <string.h>
#include <omnetpp.h>

#include "../Packets/nbPacket_m.h"

using namespace omnetpp;


class Rlc : public cSimpleModule
{
  protected:
    int lowerGateIn_;
    int lowerGateOut_;
    int upperGateIn_;
    int upperGateOut_;
    cPacketQueue upperQueue;
    cPacketQueue lowerQueue;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
};

Define_Module(Rlc);





#endif /* LAYERS_RLC_RLC_H_ */
