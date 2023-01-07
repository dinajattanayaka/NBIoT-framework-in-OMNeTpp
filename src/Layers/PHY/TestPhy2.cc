#include "../Packets/nbPacket_m.h"
#include "Phy.h"


#include "ChannelController.h"

using namespace omnetpp;



class TestPhy2 : public cSimpleModule
{
    protected:
       virtual void initialize() override;
       virtual void handleMessage(cMessage *msg) override;

    public:
       int preambleRand[20]; //to save the preamble number which is generated radomly
       int channelRel[40];  //to release channel
       int base_x_cord;
       int base_y_cord;
       int numNodes;

       cOutVector totalThroughput;
       int totalThroughputScalar;

//       cOutVector totalCollisions;
//       int totCol;



};

Define_Module(TestPhy2);


void TestPhy2::initialize()
{
    //testRadioIn_ = findGate("testRadioIn") ;

    cDisplayString& disStr = getDisplayString();
    base_x_cord = atoi(disStr.getTagArg("p",0));
    base_y_cord = atoi(disStr.getTagArg("p",1));

    EV <<"eNodeB coordinates are : "<<base_x_cord<<", "<<base_y_cord<<endl;
    ChannelController::getInstance()->setBaseStationCoord(base_x_cord, base_y_cord);

    numNodes = par("numNodes");



    //Send the first NPSS
    nbPacket *newPacket = new nbPacket("testLowerPacket");
    newPacket->setType(NPSS);
    newPacket->setSignalStrength(35);     //change this  to 20

    EV<<"transmit signal strength : " << newPacket->getSignalStrength()<<endl;

    EV << "Sending ";
    EV << newPacket->getType() << endl;


    for (int i=0; i<20; i++){
        preambleRand[i]=0;
     }

     for (int i=0; i<40; i++){
         channelRel[i]=0;
     }


    //SelfMessage to send first UNKNWN(SIB2)

    for (int j=0; j<numNodes; j++){

        char buf[40];
        sprintf(buf,"UE[%d]",j);
        cModule *targetModule = getModuleByPath(buf);
        nbPacket *braodPacket = newPacket->dup();
        sendDirect(braodPacket,targetModule,"radioIn");

    }





    delete newPacket;

    nbPacket *SelfPacket = new nbPacket("selfMsg");
    SelfPacket->setType(ENB_NPSS_TIMER);
    scheduleAt(simTime() + 0.002, SelfPacket);


    totalThroughput.setName("Total Throughput");
    totalThroughputScalar = 0;
    //Calculate traffic at each 0.1s
    nbPacket *trafficPacket = new nbPacket("trafficCounter");
    trafficPacket->setType(UE_TRAFFIC_TIMER);
    scheduleAt(simTime() + 0.1, trafficPacket);


//    totCol = 0;
//    totalCollisions.setName("Total Collisions");
}









void TestPhy2::handleMessage(cMessage *msg)
{
    nbPacket *testPacket = check_and_cast<nbPacket *>(msg);

    testPacket->setSignalStrength(35);





    if (testPacket->isSelfMessage()){


        //Initial NPSS
        if (testPacket->getType() == ENB_NPSS_TIMER) {
            testPacket->setType(UNKNOWN);
            EV << "Receiving type ";
            EV << "selfMessage" << endl;

            EV<<"eNodeB sending " <<"UNKNOWN" <<endl;

            for (int j=0; j<numNodes; j++){
                char buf[40];
                sprintf(buf,"UE[%d]",j);
                cModule *targetModule = getModuleByPath(buf);
                nbPacket *braodPacket = testPacket->dup();
                sendDirect(braodPacket,targetModule,"radioIn");

            }
            delete testPacket;

        }



        //NPSS before sending RA FEEDBACK
        else if (testPacket->getType() == ENB_PREAMBLE_TIMER) {
            testPacket->setType(UNKNOWN);
            EV << "Receiving type ";
            EV << "selfMessage" << endl;

            char buf[40];
            sprintf(buf,"UE[%d]",testPacket->getUeID());
            cModule *targetModule = getModuleByPath(buf);
            EV<<"eNodeB sending : NPSS  to "<<buf<<endl;
            sendDirect(testPacket,targetModule,"radioIn");


        }



        //NPSS before sending Contension resolution
        else if (testPacket->getType() == ENB_FEEDBACK_TIMER) {
            testPacket->setType(UNKNOWN);

//            testPacket->setUplinkChannelID(ChannelController::getInstance()->getUplinkChannel());
//            EV << "Giving UE["<<testPacket->getUeID()<<"]  the channel : "<<testPacket->getUplinkChannelID()<<endl;
            EV << "Receiving type ";
            EV << "selfMessage" << endl;


            char buf[40];
            sprintf(buf,"UE[%d]",testPacket->getUeID());
            cModule *targetModule = getModuleByPath(buf);
            EV<<"eNodeB sending : NPSS  to "<<buf<<endl;
            sendDirect(testPacket,targetModule,"radioIn");



//            EV<<"After sending contention resolution, Uplinkchannel array is : ";
//            for (int i=0; i<40; i++){
//                EV << ChannelController::getInstance()->uplinkChannel[i] << ", ";
//            }
//            EV<<endl;

        }



        else if (testPacket->getType() == ENB_UPLINK_TIMER) {

            int uplinkRelCH = testPacket->getUplinkChannelID();
            if (channelRel[uplinkRelCH] == 1){
                ChannelController::getInstance()->uplinkChannel[uplinkRelCH]=0;


                nbPacket *SelfPacket = new nbPacket("selfMsg");
                SelfPacket->setType(ENB_CON_RELEASE_TIMER);
                SelfPacket->setUeID(testPacket->getUeID());

                nbPacket *newPacket = new nbPacket("newMsg");
                newPacket->setType(NPSS);
                newPacket->setUeID(testPacket->getUeID());


                char buf[40];
                sprintf(buf,"UE[%d]",testPacket->getUeID());
                cModule *targetModule = getModuleByPath(buf);
                EV<<"eNodeB sending : NPSS  to "<<buf<<endl;
                sendDirect(newPacket,targetModule,"radioIn");
                scheduleAt(simTime() + 0.004, SelfPacket);

            }
            else {
//                EV<<"$$$$$$$$$$$$$$$$$$ ERROR $$$$$$$$$$$$$$$$$$$$"<<endl;
            }

            channelRel[uplinkRelCH] -= 1;     //reduced the message number sent to that channel by one



            EV << "After timeout from receiving a SDU from UE["<<testPacket->getUeID()<<"]  Uplinkchannel array is : ";
            for (int i=0; i<40; i++){
                EV << ChannelController::getInstance()->uplinkChannel[i] << ", ";
            }
            EV<<endl;


            EV << "After timeout from receiving a SDU from UE["<<testPacket->getUeID()<<"]  channel release array is : ";
            for (int i=0; i<40; i++){
                EV << channelRel[i] << ", ";
            }
            EV<<endl;

            delete testPacket;



        }


        else if (testPacket->getType() == ENB_CON_RELEASE_TIMER){
            testPacket->setType(UNKNOWN);

            EV << "Receiving type ";
            EV << "selfMessage" << endl;


            char buf[40];
            sprintf(buf,"UE[%d]",testPacket->getUeID());
            cModule *targetModule = getModuleByPath(buf);
            EV<<"eNodeB sending : NPSS  to "<<buf<<endl;
            sendDirect(testPacket,targetModule,"radioIn");


            EV<<"After sending Channel release message, Uplinkchannel array is : ";
            for (int i=0; i<40; i++){
                EV << ChannelController::getInstance()->uplinkChannel[i] << ", ";
            }
            EV<<endl;

        }




        else if (testPacket->getType() == UE_TRAFFIC_TIMER){
            totalThroughput.record(totalThroughputScalar/0.1);
//            EV<<"Received packets within 0.1s interval is :"<<totalThroughputScalar<<endl;
            totalThroughputScalar = 0;

            scheduleAt(simTime() + 0.1, testPacket);
        }

    }















    else if (testPacket->getType() == RA_PREAMBLE) {
        EV << "eNodeB receiving type : RA_PREAMBLE  from UE[";
        EV << testPacket->getUeID()<<"]" << endl;


        nbPacket *newPacket = new nbPacket("testLowerPacket");
        newPacket->setUeID(testPacket->getUeID());
        newPacket->setType(NPSS);





        int RAChannel = testPacket->getRAChannelID();
        if (ChannelController::getInstance()->collideChannels[RAChannel]==0){
            double randVar = 5*(testPacket->getPreambleVal())+1;
            preambleRand[RAChannel]=randVar;
        }

        else{
            bubble("Collision in Preambles");
//            totCol+=1;
//            totalCollisions.record(totCol);
        }



        EV<<"After receiving preamble, RA channel array is : ";
        for (int i=0; i<20; i++){
            EV << ChannelController::getInstance()->RAchannels[i] << ", ";
        }
        EV<<endl;

        EV<<"After receiving preamble, random value array is : ";
        for (int i=0; i<20; i++){
            EV << preambleRand[i] << ", ";
        }
        EV<<endl;


        EV<<"After recieving preamble, Collision in each channel is : ";
        for (int i=0; i<20; i++){
            EV << ChannelController::getInstance()->collideChannels[i] << ", ";
        }
        EV<<endl;



        testPacket->setPreambleVal(preambleRand[RAChannel]);
        testPacket->setType(ENB_PREAMBLE_TIMER);


        char buf[40];
        sprintf(buf,"UE[%d]",newPacket->getUeID());
        cModule *targetModule = getModuleByPath(buf);
        EV<<"eNodeB sending : NPSS  to "<<buf<<endl;
        sendDirect(newPacket,targetModule,"radioIn");
        scheduleAt(simTime() + 0.004, testPacket);      // 8 slots till NPCCH

    }









    else if (testPacket->getType() == RA_FEEDBACK) {

        EV << "eNodeB receiving type : RA_FEEDBACK  from UE[";
        EV << testPacket->getUeID()<<"]" << endl;


        nbPacket *newPacket = new nbPacket("testLowerPacket");
        newPacket->setUeID(testPacket->getUeID());
        newPacket->setType(NPSS);



        int RAChannel = testPacket->getRAChannelID();
        if (ChannelController::getInstance()->collideChannels[RAChannel]==0){
            double randVar = 5*(testPacket->getPreambleVal())+1;
            preambleRand[RAChannel]=randVar;
        }

        else{
            bubble("Collision in Feedback");
//            totCol+=1;
//            totalCollisions.record(totCol);
        }



        EV<<"After receiving feedback, RA channel array is : ";
        for (int i=0; i<20; i++){
            EV << ChannelController::getInstance()->RAchannels[i] << ", ";
        }
        EV<<endl;

        EV<<"After receiving feedback, random value array is : ";
        for (int i=0; i<20; i++){
            EV << preambleRand[i] << ", ";
        }
        EV<<endl;


        EV<<"After receiving feedback, Collision in each channel is : ";
        for (int i=0; i<20; i++){
            EV << ChannelController::getInstance()->collideChannels[i] << ", ";
        }
        EV<<endl;



        testPacket->setPreambleVal(preambleRand[RAChannel]);
        testPacket->setType(ENB_FEEDBACK_TIMER);


        char buf[40];
        sprintf(buf,"UE[%d]",newPacket->getUeID());
        cModule *targetModule = getModuleByPath(buf);
        EV<<"eNodeB sending : NPSS  to "<<buf<<endl;
        sendDirect(newPacket,targetModule,"radioIn");
        scheduleAt(simTime() + 0.004, testPacket);      // 8 slots till NPCCH


    }










    else if (testPacket->getType() == PHY_SDU){


        EV << "eNodeB receiving type : PHY_SDU  from UE[";
        EV << testPacket->getUeID()<<"]" << endl;

        int sigStrength = testPacket->getSignalStrength();

        if ( sigStrength >= eNODEB_SENSITIVITY){

            nbPacket *newPacket = new nbPacket("selfMsg");
            newPacket->setUeID(testPacket->getUeID());
            newPacket->setType(ENB_UPLINK_TIMER);

            totalThroughputScalar+=1;     //calculate received packets for 1s



            newPacket->setUplinkChannelID(testPacket->getUplinkChannelID());
            channelRel[testPacket->getUplinkChannelID()]+=1;



            EV<<"After receiving new SDU from UE["<<testPacket->getUeID()<<"]"<<",channel release array is : ";
            for (int i=0; i<40; i++){
                EV << channelRel[i] << ", ";
            }
            EV<<endl;

            scheduleAt(simTime() + 2, newPacket);


//            UeTraffic[testPacket->getUeID()].record();



        }
        else{

        }

        delete testPacket;
    }
}
