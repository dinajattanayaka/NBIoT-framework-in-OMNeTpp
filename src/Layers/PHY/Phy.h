/*
 * Phy.h
 *
 *  Created on: Oct 18, 2018
 *      Author: dinaj
 */

#ifndef LAYERS_PHY_PHY_H_
#define LAYERS_PHY_PHY_H_

#include <string.h>
#include <omnetpp.h>

#include "../Packets/nbPacket_m.h"
#include "ChannelController.h"

using namespace omnetpp;


class Phy : public cSimpleModule
{
  protected:
    int upperGateIn_;
    int upperGateOut_;
    int radioIn_;
    int uID;
    int traffic;

    double slotTime = 0.0005; // seconds
    double subFrameTime = 2 * slotTime;
    double frameTime = 10 * subFrameTime;


    double x_cord;
    double y_cord;
    int stateVar;
    int RAChannelNum;
    int feedbackChannelNum;
    int uplinkChannelNum;

    double preambleValue;
    double responseValue;
    double contensionValue;

    int slotsTillNPBCH = 4;
    int slotsTillNPCCH = 8;
    int slotsTillNPSCH = 12;

    cPacketQueue upperQueue;
    cPacketQueue lowerQueue;

    cOutVector frameRate;
    int frameRateScalar;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
};

Define_Module(Phy);




#endif /* LAYERS_PHY_PHY_H_ */
