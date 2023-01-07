#include <string.h>
#include <omnetpp.h>
#include <ctime>
#include <cstdlib>
#include <iostream>


#include "Phy.h"

#include "../Packets/nbPacket_m.h"
#include "ChannelController.h"


using namespace omnetpp;


void Phy::initialize()
{
    upperGateIn_ = findGate("upperGateIn");
    upperGateOut_ = findGate("upperGateOut");
    radioIn_ = findGate("radioIn") ;

    uID = getParentModule()->getIndex();

    traffic = par("trafficType");

    stateVar = NPBCH_INITIAL;     //Starting to receive NPSS to set timer to listen NPBCH

//    nbPacket *packetTest = new nbPacket("Test");
//    packetTest->setType(NPRACH_TIMER);
//    cModule *targetModule = getModuleByPath("TestPhy2");
//    sendDirect(packetTest,targetModule,"testRadioIn");
//    EV << "Sending test message" << endl;


    // Getting the coordinates of the channel

//    cDisplayString& disStr = getDisplayString();
//    x_cord = atoi(disStr.getTagArg("p",0));
//    y_cord = atoi(disStr.getTagArg("p",1));


    x_cord = par("x").doubleValue();
    y_cord = par("y").doubleValue();

    EV <<"Coordinates are : "<<x_cord<<", "<<y_cord<<endl;


    getParentModule()->getDisplayString().setTagArg("p",0,x_cord);
    getParentModule()->getDisplayString().setTagArg("p",1,y_cord);


    srand(time(0));


    frameRate.setName("Frame rate");
//    frameRateScalar =0;
//    //Calculate traffic at each 0.1s
//    nbPacket *trafficPacket = new nbPacket("trafficCounter");
//    trafficPacket->setType(UE_TRAFFIC_TIMER);
//    scheduleAt(simTime() + 0.1, trafficPacket);
}












void Phy::handleMessage(cMessage *msg){

    nbPacket *packetPhy = check_and_cast<nbPacket *>(msg);






/************************ Self Message for repetition *******************************/
    if (packetPhy->isSelfMessage()){
        if(packetPhy->getType()  == NPBCH_TIMER){
            stateVar = NPBCH_IDLE;
            delete packetPhy;

        }

        else if (packetPhy->getType()  == PHY_TRIGGER){
            EV<<"$$$$$$ This is PHY_TRIGGER $$$$$$"<<endl;
            delete packetPhy;

        }

        else if (packetPhy->getType()  == NPCCH_TIMER){
            if (stateVar == PREAMBLE_SENT){
                EV<<"NPCCH timer after receiving NPSS for Response" <<endl;
                stateVar = NPCCH_RA_RESPONSE_IDLE;
                delete packetPhy;
            }

            else if (stateVar == RA_FEEDBACK_SENT){
                EV<<"NPCCH timer after receiving NPSS for Contention resolution" <<endl;
                stateVar = NPCCH_CONRES_IDLE;
                delete packetPhy;
            }

            else if (stateVar == PHY_DATA_SENT){
                EV<<"NPCCH timer after receiving NPSS for PHY_DATA" <<endl;
                stateVar = CON_RELEASE_IDLE;
                delete packetPhy;
            }
        }



        // Release the channel after sending the preamble
        else if (packetPhy->getType()  == RA_CONTROLLER_TIMER){
            ChannelController::getInstance()->resetPreambleChannel(RAChannelNum);

            EV <<"Releasing RA channel" <<endl;
            delete packetPhy;

            EV<<"After releasing channel, RA channel array is : ";
            for (int i=0; i<20; i++){
                EV << ChannelController::getInstance()->RAchannels[i] << ", ";
            }
            EV<<endl;

            EV<<"After releasing channel, collison channel array is : ";
            for (int i=0; i<20; i++){
                EV << ChannelController::getInstance()->collideChannels[i] << ", ";
            }
            EV<<endl;
        }



        else if (packetPhy->getType()  == FEEDBACK_CONTROLLER_TIMER){
            ChannelController::getInstance()->resetPreambleChannel(feedbackChannelNum);

            EV <<"Releasing Feedback channel" <<endl;
            delete packetPhy;

            EV<<"After releasing channel, RA channel array is : : ";
            for (int i=0; i<20; i++){
                EV << ChannelController::getInstance()->RAchannels[i] << ", ";
            }
            EV<<endl;

            EV<<"After releasing channel, collison channel array is : ";
            for (int i=0; i<20; i++){
                EV << ChannelController::getInstance()->collideChannels[i] << ", ";
            }
            EV<<endl;
         }



//        //Calculate framerate vector
//        else if (packetPhy->getType()  == UE_TRAFFIC_TIMER){
//            frameRate.record(frameRateScalar/0.1);
////            EV<<"Received packets within 0.1s interval is :"<<frameRateScalar<<endl;
//            frameRateScalar = 0;
//
//            scheduleAt(simTime() + 0.1, packetPhy);
//        }
    }













/****************** Handling messages received from the Radio channels ********************/
    else if (packetPhy->getArrivalGate()->getId()==radioIn_){

        if(packetPhy->getType() == NPSS){

            EV<<"PHY lowerGate receiving type : NPSS"<<endl;

            if(stateVar == NPBCH_INITIAL){
                nbPacket *RAtimerPacket = new nbPacket("NPBCH");  // Narrowband Random access channel

                RAtimerPacket->setType(NPBCH_TIMER);
                scheduleAt(simTime() + slotTime * slotsTillNPBCH-0.0001, RAtimerPacket);

                delete packetPhy;

            }




            else if (stateVar == PHY_IDLE){
                delete packetPhy;
            }




            //If both signals cannot be recovered at the eNodeB
            else if (stateVar == PREAMBLE_SENT){
                EV<<"Preamble sent achieved"<<endl;     //Delete this after debug
                nbPacket *RAResponsetimerPacket = new nbPacket("NPCCH");  // Narrowband control channel

                RAResponsetimerPacket->setType(NPCCH_TIMER);
                scheduleAt(simTime() + slotTime * slotsTillNPCCH-0.0001, RAResponsetimerPacket);

                delete packetPhy;

            }



            else if (stateVar == RA_FEEDBACK_SENT){
                 nbPacket *RAFeedbackTimerPacket = new nbPacket("NPCCH");  // Narrowband control channel

                 RAFeedbackTimerPacket->setType(NPCCH_TIMER);
                 scheduleAt(simTime() + slotTime * slotsTillNPCCH-0.0001, RAFeedbackTimerPacket);

                 delete packetPhy;

             }


            else if (stateVar == PHY_DATA_SENT){
                 nbPacket *DataTimerPacket = new nbPacket("NPCCH");  // Narrowband control channel

                 DataTimerPacket->setType(NPCCH_TIMER);
                 scheduleAt(simTime() + slotTime * slotsTillNPCCH-0.0001, DataTimerPacket);

                 delete packetPhy;

             }

        }








        else if(packetPhy->getType() == UNKNOWN){
            EV<<"PHY lowerGate receiving type : UNKNOWN" <<endl;

            if(stateVar == NPBCH_IDLE){

                double RxSigStrength = packetPhy->getSignalStrength();
                EV<<"Signal Strength before fading : " <<RxSigStrength<< endl;
//                double fc = packetPhy->getCarrierFreq();
//                EV<<"Carrier frequency" <<endl;
                double fc = 925;
                EV<<"Carrier frequency : " << fc <<endl;

                double newSigStrength =ChannelController::getInstance()->getAttenuatedSigStrength(RxSigStrength, fc, x_cord, y_cord );
                EV<<"Received signal strength : " <<newSigStrength<<endl;
                packetPhy->setSignalStrength(newSigStrength);
                packetPhy->setType(SIB2);

                send(packetPhy,"upperGateOut");
                EV<<"UpperGate sending Type : SIB2 " <<endl;

                stateVar = PHY_IDLE;

                nbPacket *TriggerPacket = new nbPacket("PHY_TRIGGER"); // Wait until packet comes from upper layer
                TriggerPacket->setType(PHY_TRIGGER);

    //            scheduleAt(simTime() + slotTime * slotsTillNPBCH-0.0001, TriggerPacket);    //add random time
            }


            else if (stateVar == PHY_IDLE){
                delete packetPhy;
            }




            // Wait until RA feedback from MAC layer
            else if(stateVar == NPCCH_RA_RESPONSE_IDLE){
                packetPhy->setType(RA_RESPONSE);

                responseValue = packetPhy->getPreambleVal();  //get the value sent by the base station

                send(packetPhy,"upperGateOut");
                EV<<"UpperGate sending Type : RA_RESPONSE " <<endl;

                stateVar = NPCCH_RA_FEEDBACK_IDLE;
            }



            else if(stateVar == NPCCH_CONRES_IDLE){



                if(preambleValue == packetPhy->getPreambleVal()){
                    packetPhy->setType(RA_CONTENSION_RESOLUTION);

                    //change the value to 1 in the relevant position channel controller array

                    packetPhy->setUplinkChannelID(ChannelController::getInstance()->getUplinkChannel());
                    EV << "Giving UE["<<packetPhy->getUeID()<<"]  the channel : "<<packetPhy->getUplinkChannelID()<<endl;

                    EV<<"After sending contention resolution, Uplinkchannel array is : ";
                    for (int i=0; i<40; i++){
                        EV << ChannelController::getInstance()->uplinkChannel[i] << ", ";
                    }
                    EV<<endl;



                    uplinkChannelNum = packetPhy->getUplinkChannelID();
                    send(packetPhy,"upperGateOut");
                    EV<<"PHY upperGate sending Type : RA_CONTENSION_RESOLUTION " <<endl;

                    stateVar = NPUSCH_IDLE;
                }
                else {
                    packetPhy->setType(RA_CONTENTION_MISMATCH);

                    send(packetPhy,"upperGateOut");
                    EV<<"PHY upperGate sending Type : RA_CONTENTION_MISMATCH " <<endl;
                    stateVar = PREAMBLE_SENT;  ////////check the state correctly
                }
            }


            else if(stateVar == CON_RELEASE_IDLE){
                packetPhy->setType(CONNECTION_RELEASE);

                send(packetPhy,"upperGateOut");
                EV<<"PHY upperGate sending Type : CONNECTION_RELEASE " <<endl;

                stateVar = PHY_IDLE;
            }
        }
    }








/****************** Handling messages received from the upper gate ***********************/
    else if (packetPhy->getArrivalGate()->getId()==upperGateIn_){
        if(packetPhy->getType() == TAU){

        }

        else if (packetPhy->getType() == RA_PREAMBLE){

            if(stateVar == PHY_IDLE || stateVar == PREAMBLE_SENT){              /////////////PHY_IDLE or NPBCH_IDLE ??
                //get a channel from channel controller
                //change the message variable channel to the given value

                EV<<"PHY upperGate receiving type: RA_PREAMBLE" <<endl;

                RAChannelNum = (rand()%20);
                ChannelController::getInstance()->setPreambleChannel(RAChannelNum);
                packetPhy->setRAChannelID(RAChannelNum);
                EV<<"RA Preamble channel is : "<<RAChannelNum<<endl;



                EV<<"After selecting RA preamble channel, RA channel array is : ";
                for (int i=0; i<20; i++){
                    EV << ChannelController::getInstance()->RAchannels[i] << ", ";
                }
                EV<<endl;

                EV<<"After electing RA preamble channel, collison channel array is : ";
                for (int i=0; i<20; i++){
                    EV << ChannelController::getInstance()->collideChannels[i] << ", ";
                }
                EV<<endl;




                preambleValue = (rand()%60);
                packetPhy->setPreambleVal(preambleValue);
                EV<<"Random Preamble value is : "<<preambleValue<<endl;


                double UEtxStrength = 23;
                double frequency = 880;
                EV<<"Carrier frequency : " << frequency <<endl;
                double newSigStrength =ChannelController::getInstance()->getAttenuatedSigStrength(UEtxStrength, frequency, x_cord, y_cord );
                EV<<"eNodeB received strength from UE["<<uID<<"] : " <<newSigStrength<<endl;
                packetPhy->setSignalStrength(newSigStrength);



                cModule *targetModule = getModuleByPath("eNodeB"); //get the traget module to send preamble


                nbPacket *RAControlerTimer = new nbPacket("RA_CONTROLLER_TIMER");  // timer to release channel

                RAControlerTimer->setType(RA_CONTROLLER_TIMER);

                packetPhy->setUeID(uID);
                packetPhy->setTrafficType(traffic);
                sendDirect(packetPhy,targetModule,"testRadioIn");
                scheduleAt(simTime() + 0.001, RAControlerTimer);

                EV<<"PHY lowerGate sending type : RA Preamble" <<endl;
                EV << "Sending RA channel release Self message" << endl;

                stateVar = PREAMBLE_SENT;


            }
        }


        else if (packetPhy->getType() == RA_FEEDBACK){

            if(stateVar == NPCCH_RA_FEEDBACK_IDLE || stateVar == RA_FEEDBACK_SENT){
                //get a channel from channel controller
                //change the message variable channel to the given value
                //add a random number to uniquely ID the node


                EV<<"PHY upperGate receiving type: RA_FEEDBACK" <<endl;


                feedbackChannelNum = (rand()%20);
                ChannelController::getInstance()->setPreambleChannel(feedbackChannelNum);
                packetPhy->setRAChannelID(feedbackChannelNum);

                EV<<"RA Feedback channel is : "<<feedbackChannelNum<<endl;


                EV<<"After selecting RA feedback channel, RA channel array is : ";
                for (int i=0; i<20; i++){
                    EV << ChannelController::getInstance()->RAchannels[i] << ", ";
                }
                EV<<endl;

                EV<<"After electing RA feedback channel, collison channel array is : ";
                for (int i=0; i<20; i++){
                    EV << ChannelController::getInstance()->collideChannels[i] << ", ";
                }
                EV<<endl;



                //change the preamble value
                contensionValue = (responseValue - 6)/25;
                packetPhy->setPreambleVal(contensionValue);
                EV<<"Changed Preamble function value is : "<<contensionValue<<endl;


                double UEtxStrength = 23;
                double frequency = 880;
                EV<<"Carrier frequency : " << frequency <<endl;
                double newSigStrength =ChannelController::getInstance()->getAttenuatedSigStrength(UEtxStrength, frequency, x_cord, y_cord );
                EV<<"eNodeB received strength from UE["<<uID<<"] : " <<newSigStrength<<endl;
                packetPhy->setSignalStrength(newSigStrength);


                packetPhy->setUeID(uID);
                packetPhy->setTrafficType(traffic);
                cModule *targetModule = getModuleByPath("eNodeB");
                sendDirect(packetPhy,targetModule,"testRadioIn");


                nbPacket *feedbackControlerTimer = new nbPacket("FEEDBACK_CONTROLLER_TIMER");  // timer to release channel

                feedbackControlerTimer->setType(FEEDBACK_CONTROLLER_TIMER);
                 scheduleAt(simTime() + 0.001, feedbackControlerTimer);


                EV << "Sending feedback channel release message" << endl;

                stateVar = RA_FEEDBACK_SENT;


            }
        }



        else if (packetPhy->getType() == MAC_SDU){

            if(stateVar == NPUSCH_IDLE || stateVar == PHY_DATA_SENT){

                EV<<"PHY upperGate receiving Type: MAC_SDU" <<endl;
                packetPhy->setType(PHY_SDU);
                packetPhy->setUplinkChannelID(uplinkChannelNum); //Add the uplink channel ID


                double UEtxStrength = 23;
                double frequency = 880;
                EV<<"Carrier frequency : " << frequency <<endl;
                double newSigStrength =ChannelController::getInstance()->getAttenuatedSigStrength(UEtxStrength, frequency, x_cord, y_cord );
                EV<<"eNodeB received strength from UE["<<uID<<"] : " <<newSigStrength<<endl;
                packetPhy->setSignalStrength(newSigStrength);

//                if (newSigStrength>=eNODEB_SENSITIVITY){
//                    frameRateScalar+=1;
//                }


                cModule *targetModule = getModuleByPath("eNodeB");
                packetPhy->setUeID(uID);
                packetPhy->setTrafficType(traffic);
                sendDirect(packetPhy,targetModule,"testRadioIn");
                EV << "PHY lowerGate sending type : PHY_SDU" << endl;

                stateVar = PHY_DATA_SENT;
            }
        }
    }
}

