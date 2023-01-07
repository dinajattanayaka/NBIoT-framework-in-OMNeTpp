
#ifndef LAYERS_MAC_MAC_H_
#define LAYERS_MAC_MAC_H_

#include <string.h>
#include <omnetpp.h>

#include "../Packets/nbPacket_m.h"


using namespace omnetpp;



class Mac : public cSimpleModule
{
  protected:

    // Integer values for the gates
    int lowerGateIn_;
    int lowerGateOut_;
    int upperGateIn_;
    int upperGateOut_;

    int schedulingFrq;
    int sigStrength;
    int numOfRepetitions;
    int framesperPacket;

    int batteryPowerState;
    int conTrigger;
    long newPsmID;
    double powerLevel;
    simtime_t calTime;
    double cumPower;

    simtime_t scheduleTime;
    cMessage *timeoutEvent;
    int preambleCounter;
    int feedbackCounter;
    int raResponseState;
    int contResState;

    simtime_t SDUscheduleTime;
    int sduCounter;

    cPacketQueue upperQueue; //to receive data messages from lower gate
    cPacketQueue lowerQueue;
    cPacketQueue macQueue;
    cPacketQueue sigQueue;
    cPacketQueue contentionQueue;




 //   additionalInfo adObj;

  protected:
    virtual void handleMessage(cMessage *msg) override;
    virtual void initialize() override;
    int checkCElevel(cMessage *msg);
    void RAPreambleRepetition(int repFreq, simtime_t scheduleT, cMessage *msg);
    void RAPreambleTransmit(simtime_t scheduleT, cMessage *msg);
    void RAFeedbackRepetition(int repFreq, simtime_t scheduleT, cMessage *msg);
    void RAFeedbackTransmit(simtime_t scheduleT, cMessage *msg);
    void SDUTransmit(simtime_t scheduleT, cMessage *msg);
    void SDURepetition(int repFreq, simtime_t scheduleT, cMessage *msg);

  public :
    Mac();
    virtual ~Mac();
    cOutVector UEpower;

    cOutVector packetdelay;
    simtime_t packetdelaycounter;

};

Define_Module(Mac);



#endif /* LAYERS_MAC_MAC_H_ */
