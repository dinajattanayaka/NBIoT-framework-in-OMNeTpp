#include "ChannelController.h"
#include "Phy.h"
#include <vector>

#include "../Packets/nbPacket_m.h"

Define_Module(ChannelController);


ChannelController *ChannelController::instance = nullptr;

//template<typename T>
//bool contains(const std::vector<T>& vector, T item) { return std::find(vector.begin(), vector.end(), item) != vector.end(); }




ChannelController::ChannelController()
{
    if (instance)
        throw cRuntimeError("There can be only one ChannelController instance in the network");
    instance = this;
    channel_state = false;
//    environment = URBAN;

    //intialize the channel and collision arrays
    for (int i=0; i<20; i++){
        RAchannels[i]=0;
        collideChannels[i]=0;
    }

    for (int i=0; i<40; i++){
        uplinkChannel[i]=0;
    }

}




ChannelController::~ChannelController()
{
    instance = nullptr;
}



ChannelController *ChannelController::getInstance()
{
    if (!instance)
        throw cRuntimeError("ChannelController::getInstance(): there is no ChannelController module in the network");
    return instance;
}



void ChannelController::initialize()
{
    totCol = 0;
    totalCollisions.setName("Total Collisions");

    environment = par("netEnv");   //URBAN-70 , SUBURBAN-71, RURAL-72
}



//
//void ChannelController::addNode(Phy *node)
//{
//    nodes.push_back(node);
//}
//
//
//
//void ChannelController::start_tx(Phy *node){
//    tx_nodes.push_back(node);
//    channel_state = true;
//}



//bool ChannelController::getChannelState(Phy *node){
//
//
//
//    int x1 = atoi(node->getParentModule()->getDisplayString().getTagArg("p",0));
//    int y1 = atoi(node->getParentModule()->getDisplayString().getTagArg("p",1));
//
//
//    for (int i = 0; i < (int)tx_nodes.size(); i++){
//
//        int x2 = atoi(tx_nodes[i]->getParentModule()->getDisplayString().getTagArg("p",0));
//        int y2 = atoi(tx_nodes[i]->getParentModule()->getDisplayString().getTagArg("p",1));
//
//        int rec_dis=sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) );
//
//        if(rec_dis < intefearence_range){
//            return true;
//        }
//    }
//
//    return false;
//
//    return channel_state;
//}



//void ChannelController::end_tx(Phy *node){
//
//    for (int i = 0; i < (int)tx_nodes.size(); i++){
//        if (tx_nodes[i] == node){
//            tx_nodes.erase(tx_nodes.begin()+i);
//        }
//    }
//    channel_state = false;
//}







void ChannelController::handleMessage(cMessage *msg)
{
    throw cRuntimeError("This module does not process messages");

}





// get the base station coordinates
void ChannelController::setBaseStationCoord(double base_x, double base_y){
    base_x_cord = base_x;
    base_y_cord = base_y;
}


double ChannelController::getAttenuatedSigStrength(double RxSigStrength, double fc, double x_cord, double y_cord ){
    double d = sqrt((base_x_cord - x_cord)*(base_x_cord -x_cord ) + (base_y_cord-y_cord)*(base_y_cord-y_cord));
    double d_km = d/1000;
    EV <<"Distance is : "<<d_km<<endl;

    EV<<"Environment is : "<<environment<<endl;

    double hT = 30 ;  //50
    double hR = 1.5 ;   //5
    double K = 36;  //K ranges from 35.94 (countryside) to 40.94 (desert)
    double aHr;
    double ahx;

    double otherLoss = 27;

    ahx = (1.1*log10(fc) - 0.7)*hR - (1.56*log10(fc) - 0.8);

    if (fc>300){
        aHr = 3.2*(log10(11.75*hR)*log10(11.75*hR)) - 4.97;
    }
    else {
        aHr = 8.29*(log10(1.54*hR)*log10(1.54*hR)) - 1.1;
    }


    double UrbanPathLoss = 69.55 + 26.16*log10(fc) - 13.82*log10(hT) - aHr + (44.9 - 6.55*log10(hT))*log10(d_km)+otherLoss;
    double SubUrbanPathLoss = UrbanPathLoss - 2*log10(fc/28)*log10(fc/28) - 5.4;
    double RuralPathLoss = UrbanPathLoss - 4.78*log10(fc)*log10(fc) + 18.33*log10(fc) - K  ;



    double strengthAfterLoss;
    double totalLoss;
    if (environment == URBAN){
        totalLoss = normal(UrbanPathLoss,10);     //change it to 20
        EV<<"Path Loss is :"<<UrbanPathLoss<<endl;
        EV<<"Total Loss is :"<<totalLoss<<endl;
        strengthAfterLoss = RxSigStrength - totalLoss;
    }

    else if (environment == SUBURBAN){
        totalLoss = normal(SubUrbanPathLoss,10);   //change it to 20
        EV<<"Path Loss is :"<<UrbanPathLoss<<endl;
        EV<<"Total Loss is :"<<totalLoss<<endl;
        strengthAfterLoss = RxSigStrength - totalLoss;
    }

    else if (environment == RURAL){
        totalLoss = normal(RuralPathLoss,10);      //change it to 20
        EV<<"Path Loss is :"<<UrbanPathLoss<<endl;
        EV<<"Total Loss is :"<<totalLoss<<endl;
        strengthAfterLoss = RxSigStrength - totalLoss;
    }

    return strengthAfterLoss;

}



//when accessing the channel
void ChannelController::setPreambleChannel(int chNum){
    if (RAchannels[chNum]==0){
        RAchannels[chNum]=1;     //No collisons detected

    }

    else{
        collideChannels[chNum]+=1;
        totCol+=1;
        totalCollisions.record(totCol);
    }
}



void ChannelController::resetPreambleChannel(int chNum){
    if (collideChannels[chNum]==0){
        RAchannels[chNum]=0;     //No collisons detected

    }

    else{
        collideChannels[chNum]-=1;
    }
}


int ChannelController::getUplinkChannel(){
    for (int i=0; i<40; i++){
        if(uplinkChannel[i]==0){
            uplinkChannel[i]=1;
            return i;
            break;
        }
    }
}


