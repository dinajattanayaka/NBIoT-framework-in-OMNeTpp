
#ifndef LAYERS_PHY_CHANNELCONTROLLER_H_
#define LAYERS_PHY_CHANNELCONTROLLER_H_




#include <omnetpp.h>
#include "Phy.h"
#include <math.h>


using namespace omnetpp;

/**
 * This module is responsible for tracking the distance of mobile nodes
 */



class ChannelController : public cSimpleModule
{
  protected:

    static ChannelController *instance;
//    std::vector<Mac *> nodes;
//    std::vector<Mac *> tx_nodes;

//    std::vector<Satellite *> satellites

//    std::vector<GroundStation *> stations;
//
//    std::vector<osgEarth::Util::LinearLineOfSightNode *> losNodes;
//    std::map<Satellite *, osg::Geometry *> orbitsMap;
//
//    osg::ref_ptr<osg::Geode> connections = nullptr;



    bool channel_state = false;
    int intefearence_range = 250; //m




  public:
    double base_x_cord;
    double base_y_cord;
    int environment;

    int RAchannels[20];
    int collideChannels[20];
    int uplinkChannel[40];

    cOutVector totalCollisions;
    int totCol;



  protected:
    virtual void initialize() override;
    virtual int numInitStages() const override { return 3; }
    virtual void handleMessage(cMessage *msg) override;

  public:
    ChannelController();
    virtual ~ChannelController();
    static ChannelController *getInstance();
//    void addNode(Phy *node); // to be called exactly in initialize stage 1
//    void start_tx(Phy *node);
//    void end_tx(Phy *node);
//    bool getChannelState(Phy *node);


    /****** UE ****/
    double getAttenuatedSigStrength(double RxSigStrength, double fc, double x_cord, double y_cord);
    void setBaseStationCoord(double base_x, double base_y);
    void setPreambleChannel(int chNum);
    void resetPreambleChannel(int chNum);


    /***** eNodeB *****/

    int getUplinkChannel();
    //    void addGroundStation(GroundStation *p); // to be called exactly in initialize stage 1
};




#endif /* LAYERS_PHY_CHANNELCONTROLLER_H_ */
