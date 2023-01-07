#include <string.h>
#include <omnetpp.h>
#include "Mac.h"

using namespace omnetpp;



Mac::Mac(){
    timeoutEvent = nullptr;
}


Mac::~Mac(){
    cancelAndDelete(timeoutEvent);
}



void Mac::initialize()
{
    lowerGateIn_ = findGate("lowerGateIn");
    lowerGateOut_ = findGate("lowerGateOut");
    upperGateIn_ = findGate("upperGateIn");
    upperGateOut_ = findGate("upperGateOut");
    framesperPacket = 20;

    scheduleTime = 0.005;
    SDUscheduleTime = 0.005;


    preambleCounter = 0; //Count the number of RA preambles sent
    feedbackCounter = 0; //Count the number of RA feedback sent
    sduCounter = 0;
    raResponseState = 0; //Change the state to 1 when receiving RA response
    contResState = 0;    //Change the state to 1 when receiving Contention resolution
    conTrigger =0;       //To remove the previous self message from repetition
    calTime = 0;         //Store simulation time to calculate time differences
    cumPower = BATTERY_CAPACITY; //Cumulative power
    powerLevel = 0;     //Change power levels Tx, Rx, Idle, Psm



    UEpower.setName("UE power");

    packetdelaycounter = 0;
    packetdelay.setName("Message delay");



    nbPacket *psPacket = new nbPacket("psPacket");    //Starting receiving level
    psPacket->setType(IDLE1);
    scheduleAt(simTime()+10, psPacket);
    calTime = simTime();                             //storing current time
    powerLevel = IDLE_POWER_LEVEL;                   //Changing power level


    EV << "Scheduling Power saving timer"<<endl;
    EV << "Sending IDLE1 "<<endl;
    newPsmID = psPacket->getId() ;                  //getting the ID
    EV<<"Sending ID is :"<<newPsmID<<endl;


//    nbPacket *tauPacket = new nbPacket("tauPacket");
//    tauPacket->setType(TAU);
//
//    sendDelayed (tauPacket,0.001,"lowerGateOut");
    cumPower = cumPower - 0.001*(TX_POWER_LEVEL-IDLE_POWER_LEVEL);  //Reducing battery for transmission
    EV<<"New Power Level is " <<cumPower<<endl;
    EV << "MAC lowerGate sending type : TAU" << endl;

}


void Mac::handleMessage(cMessage *msg)
{
    nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
    schedulingFrq = packetMac->getSchedulingFrequency() ;
//    EV<< "Scheduling frequency is ";
//    EV <<schedulingFrq << endl;



//    sigStrength = packetMac->getSignalStrength() ;
//    EV <<"Signal strength is ";
//    EV << sigStrength <<endl;




/****************** Self Message for repetition ***********************/

    if (packetMac->isSelfMessage()){

        if (packetMac->getType() == IDLE1){
            EV<<"Receiving power message type is: IDLE1"<<endl;
            if (packetMac->getId() == newPsmID){
                if(conTrigger==0){
                    if (lowerQueue.isEmpty()){
                        packetMac->setType(IDLE2);
                        scheduleAt(simTime() + 10,packetMac);

                        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
                        calTime = simTime();
                        powerLevel = IDLE_POWER_LEVEL;
                        cumPower = cumPower - 0.001*(RX_POWER_LEVEL-IDLE_POWER_LEVEL);
                        EV<<"New Power Level after receiving IDLE1 is " <<cumPower<<endl;

                        EV << "Sending IDLE2 "<<endl;

                    }
                    else {

                    }
                }
                else{
                    delete packetMac;
                    EV <<"Deleting IDLE1 because a message has arrived from Upper Layer" <<endl;
                }
            }
            else{
                delete packetMac;
                EV <<"Deleting IDLE1 because new power cycle has started" <<endl;
            }

        }



        else if (packetMac->getType() == IDLE2){
            EV<<"Receiving power message type is:" <<packetMac->getId()<<endl;
            if (packetMac->getId() == newPsmID){
                if(conTrigger==0){
                    if (lowerQueue.isEmpty()){
                        packetMac->setType(IDLE3);
                        scheduleAt(simTime() + 10,packetMac);

                        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
                        calTime = simTime();
                        powerLevel = IDLE_POWER_LEVEL;
                        cumPower = cumPower - 0.001*(RX_POWER_LEVEL-IDLE_POWER_LEVEL);
                        EV<<"New Power Level after receiving IDLE2 is " <<cumPower<<endl;

                        EV << "Sending IDLE3 "<<endl;
                    }
                    else {

                    }
                }
                else{
                    delete packetMac;
                    EV <<"Deleting IDLE2 because a message has arrived from Upper Layer" <<endl;
                }
            }
            else{
                delete packetMac;
                EV <<"Deleting IDLE2 because new power cycle has started" <<endl;
            }

        }


        else if (packetMac->getType() == IDLE3){
            EV<<"Receiving power message type is:" <<packetMac->getId()<<endl;
            if (packetMac->getId() == newPsmID){
                if(conTrigger==0){
                    if (lowerQueue.isEmpty()){
                        packetMac->setType(PSM_TRIG);
                        scheduleAt(simTime() + 3600*24,packetMac);

                        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
                        calTime = simTime();
                        powerLevel = PSM_POWER_LEVEL;
                        cumPower = cumPower - 0.001*(RX_POWER_LEVEL-IDLE_POWER_LEVEL);
                        EV<<"New Power Level after receiving IDLE2 is " <<cumPower<<endl;

                        EV << "Sending PSM_TRIG "<<endl;
                    }
                    else {

                    }
                }
                else{
                    delete packetMac;
                    EV <<"Deleting IDLE3 because a message has arrived from Upper Layer" <<endl;
                }
            }
            else{
                delete packetMac;
                EV <<"Deleting IDLE3 because new power cycle has started" <<endl;
            }

        }



        else if (packetMac->getType() == PSM_TRIG){
            EV<<"Receiving power message type is:" <<packetMac->getId()<<endl;
            if (packetMac->getId() == newPsmID){
                if(conTrigger==0){
                    if (lowerQueue.isEmpty()){
                        packetMac->setType(IDLE1);
                        scheduleAt(simTime() + 100,packetMac);   //initially

                        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
                        calTime = simTime();
                        powerLevel = IDLE_POWER_LEVEL;
                        EV << "Sending IDLE1 "<<endl;

//                        nbPacket *tauPacket = new nbPacket("tauPacket");
//                        tauPacket->setType(TAU);
//
//                        sendDelayed (tauPacket,0.001,"lowerGateOut");
                        cumPower = cumPower - 0.001*(TX_POWER_LEVEL-IDLE_POWER_LEVEL);  //Reducing battery capacity
                        EV<<"New Power Level after receiving PSM_TRIG is" <<cumPower<<endl;
                        EV << "MAC lowerGate sending type : TAU" << endl;


                    }
                    else {

                    }
                }
                else{
                    delete packetMac;
                    EV <<"Deleting PSM_TRIG because a message has arrived from Upper Layer" <<endl;
                }
            }
            else{
                delete packetMac;
                EV <<"Deleting PSM_TRIG because new power cycle has started" <<endl;
            }

        }







        //Repeat the RA preamble after receiving the first one
        else if (packetMac->getType() == RA_PREAMBLE){
            if (sigStrength>= NORMAL_COVERAGE){
                numOfRepetitions = 8 * framesperPacket;
                if (raResponseState ==0){
                    RAPreambleRepetition(numOfRepetitions,scheduleTime,packetMac);
                }

                else {
                    cancelAndDelete(packetMac);
                }
            }


            else if (NORMAL_COVERAGE> sigStrength && sigStrength>= ROBUST_COVERAGE){
                numOfRepetitions = 32 * framesperPacket;
                if (raResponseState ==0){
                    RAPreambleRepetition(numOfRepetitions,scheduleTime,packetMac);
                }

                else {
                    cancelAndDelete(packetMac);
                }
            }


            else if (ROBUST_COVERAGE> sigStrength && sigStrength>= EXTREME_COVERAGE){
                numOfRepetitions = 64 * framesperPacket;
                if (raResponseState ==0){
                    RAPreambleRepetition(numOfRepetitions,scheduleTime,packetMac);
                }

                else {
                    cancelAndDelete(packetMac);
                }
            }
        }



        //Repeat the RA feedback after receiving the first one
        else if (packetMac->getType() == RA_FEEDBACK){
            if (sigStrength>= NORMAL_COVERAGE){
                numOfRepetitions = 8 * framesperPacket;
                if (contResState ==0){
                    RAFeedbackRepetition(numOfRepetitions,scheduleTime,packetMac);
                }

                else if (contResState == 2){
//                    scheduleTime = 0.005;                               //repetition period
                    packetMac->setType(RA_PREAMBLE);
                    RAPreambleTransmit(scheduleTime,packetMac);
                    contResState = 0;
                }

                else {
                    cancelAndDelete(packetMac);
                }

            }


            else if (NORMAL_COVERAGE> sigStrength && sigStrength>= ROBUST_COVERAGE){
                numOfRepetitions = 32 * framesperPacket;
                if (contResState ==0){
                    RAFeedbackRepetition(numOfRepetitions,scheduleTime,packetMac);
                }

                else if (contResState ==2){
//                    scheduleTime = 0.005;                               //repetition period
                    packetMac->setType(RA_PREAMBLE);
                    RAPreambleTransmit(scheduleTime,packetMac);
                    contResState = 0;
                }
                else {
                    cancelAndDelete(packetMac);
                }
            }


            else if (ROBUST_COVERAGE> sigStrength && sigStrength>= EXTREME_COVERAGE){
                numOfRepetitions = 64 * framesperPacket;
                if (contResState ==0){
                    RAFeedbackRepetition(numOfRepetitions,scheduleTime,packetMac);
                }

                else if (contResState ==2){
//                    scheduleTime = 0.005;                               //repetition period
                    packetMac->setType(RA_PREAMBLE);
                    RAPreambleTransmit(scheduleTime,packetMac);
                    contResState = 0;
                }

                else {
                    cancelAndDelete(packetMac);
                }
            }
        }



        //repetition after backoff period
        else if (packetMac->getType() == RA_BACKOFF){
            if (raResponseState ==0){
//                scheduleTime = 0.005;
                packetMac->setType(RA_PREAMBLE);
                RAPreambleTransmit(scheduleTime,packetMac);
            }
            else if (contResState ==0){
//                scheduleTime = 0.005;
                packetMac->setType(RA_FEEDBACK);
                RAFeedbackTransmit(scheduleTime,packetMac);
            }
            else {
                EV<<"Deleting Backoff self message"<<endl;
                cancelAndDelete(packetMac);
            }
        }



        //Stop the Backoff of RA preamble and RA feedback
        else if (packetMac->getType() == RA_COMPLETE){
            cancelAndDelete(packetMac);
        }



        //Repeat the SDU after receiving the first one
        else if (packetMac->getType() == MAC_SDU){
            if (sigStrength>= NORMAL_COVERAGE){
                numOfRepetitions = 8 * framesperPacket;
                SDURepetition(numOfRepetitions,SDUscheduleTime,packetMac);
            }


            else if (NORMAL_COVERAGE> sigStrength && sigStrength>= ROBUST_COVERAGE){
                numOfRepetitions = 32 * framesperPacket;
                SDURepetition(numOfRepetitions,SDUscheduleTime,packetMac);
            }


            else if (ROBUST_COVERAGE> sigStrength && sigStrength>= EXTREME_COVERAGE){
                numOfRepetitions = 64 * framesperPacket;
                SDURepetition(numOfRepetitions,SDUscheduleTime,packetMac);
            }
        }




//        else if (packetMac->getType() == RA_MISMATCH_RECOVER){
//            packetMac->setType(RA_PREAMBLE);
//            RAPreambleTransmit(scheduleTime,packetMac);
//        }

    }










/****************** Handling messages received from the Lower gate ***********************/

    else if (packetMac->getArrivalGate()->getId()==lowerGateIn_){


        if (packetMac->getType() == RA_CONTENSION_RESOLUTION){
            packetMac->setType(CONNECTION_SETUP);

            EV <<"MAC lowerGate receiving Type : RA_CONTENSION_RESOLUTION"<<endl;

            contResState = 1;
            feedbackCounter = 0;

            if (!macQueue.isEmpty()){
                delete macQueue.pop();
            }


            send(packetMac, "upperGateOut");
            EV << "MAC UpperGate Sending type ";
            EV << "CONNECTION_SETUP" << endl;

            cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
            calTime = simTime();
            powerLevel = TX_POWER_LEVEL;
//            EV << "Sending IDLE1 "<<endl;
        }





        //When the preamble values mismatch in the physical layer
        else if (packetMac->getType() == RA_CONTENTION_MISMATCH){

            EV <<"MAC lowerGate receiving Type : RA_CONTENSION_MISMATCH"<<endl;


            contResState = 2;     //Error state
            feedbackCounter = 0;

            raResponseState = 0;
            preambleCounter = 0;

            if (!macQueue.isEmpty()){
                delete macQueue.pop();
            }



            cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
            calTime = simTime();
            powerLevel = TX_POWER_LEVEL;
//            EV << "Sending IDLE1 "<<endl;



        }






        else if (packetMac->getType() == MAC_PDU){

            EV <<"MAC lowerGate receiving Type : MAC_PDU"<<endl;

            packetMac->setType(RLC_PDU);
            send(packetMac, "upperGateOut");
            EV << "MAC upperGate sending type ";
            EV << "RLC_PDU" << endl;


        }



        /****** Starting the Random feedback procedure  *********/
        else if(packetMac->getType() == RA_RESPONSE){

            EV <<"MAC lower gate receiving type : RA_RESPONSE"<<endl;
            if (sigStrength< EXTREME_COVERAGE){
                delete packetMac;
            }
            else{

                preambleCounter = 0;
                if (macQueue.isEmpty()){


                    nbPacket *chePacket = packetMac->dup();
                    macQueue.insert(packetMac);
                    raResponseState=1;
//                    scheduleTime = 0.005;                               //repetition period

                    chePacket->setType(RA_FEEDBACK);
                    RAFeedbackTransmit(scheduleTime,chePacket);

                }
                else{
                    nbPacket* chePacket = check_and_cast<nbPacket*>(macQueue.pop());
                    if (chePacket->getType() == RA_RESPONSE){
                        delete packetMac;

                    }
                    else{
                        EV<<"Invalid Type : ";
                        EV<<chePacket->getType()<< endl;
                    }
                }
            }


        }


        //update the signal strength
        else if(packetMac->getType() == SIB2){
            EV <<"MAC lower gate receiving type : SIB2"<<endl;

            sigStrength = packetMac->getSignalStrength() ;
            EV <<"Signal strength is ";
            EV << sigStrength <<endl;
            sigQueue.insert(packetMac);

        }


        //Release the connection and go back to PSM mode
        else if (packetMac->getType() == CONNECTION_RELEASE){
            EV <<"MAC lower gate receiving type : CONNECTION_RELEASE"<<endl;

            conTrigger = 0;

            nbPacket *psPacket = new nbPacket("psPacket");
            psPacket->setType(PSM_TRIG);
            newPsmID = psPacket->getId() ;
            EV<<"New PSM ID is:"<<newPsmID<<endl;
            scheduleAt(simTime()+3600*24, psPacket);

            cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
            calTime = simTime();
            powerLevel = PSM_POWER_LEVEL;
            cumPower = cumPower - 0.001*(RX_POWER_LEVEL-IDLE_POWER_LEVEL);
            EV<<"New Power Level is " <<cumPower<<endl;

            EV << "Scheduling Power saving timer"<<endl;
            EV << "Sending PSM_TRIG "<<endl;


            send(packetMac, "upperGateOut");
            EV << "MAC upperGate sending type ";
            EV << "CONNECTION_RELEASE" << endl;


            raResponseState = 0;
            contResState = 0;

        }
    }












/****************** Handling messages received from the Upper gate **********************/

    else if (packetMac->getArrivalGate()->getId()==upperGateIn_){

        conTrigger = 1;


        //When data packet is received from the upper layer
        if (packetMac->getType() == RLC_SDU){

            EV <<"MAC UpperGate receiving Type : RLC_SDU"<<endl;
            packetMac->setType(MAC_SDU);
//            SDUscheduleTime = 0.005;

            cumPower = cumPower -200;      //Processing power
            SDUTransmit(SDUscheduleTime,packetMac);

        }



        // Starting Random Access procedure
        else if(packetMac->getType() == CONNECTION_SETUP_REQUEST){
            EV <<"MAC UpperGate receiving Type : CONNECTION_SETUP_REQUEST"<<endl;

//            scheduleTime = 0.005;                               //repetition period
            packetMac->setType(RA_PREAMBLE);
            RAPreambleTransmit(scheduleTime,packetMac);

            packetdelaycounter = simTime();

        }
    }
}













/****************** Functions ***********************/


int Mac::checkCElevel(cMessage *msg){
    nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
    sigStrength = packetMac->getSignalStrength() ;

    if (sigStrength>= -80){
        return NORMAL_COVERAGE;
    }
    else if (-80> sigStrength && sigStrength>= -90){
        return ROBUST_COVERAGE;
    }
    else if (-90> sigStrength && sigStrength>= -100){
        return EXTREME_COVERAGE;
    }
}




/************* Send the first random access message to trigger repetition ***************/

void Mac::RAPreambleTransmit(simtime_t scheduleT, cMessage *msg){
    nbPacket *packetMac = check_and_cast<nbPacket *>(msg);

    if (sigStrength>= NORMAL_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        preambleCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << packetMac->getType();
        EV <<" :  Packet no " << preambleCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);


        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();

        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;



    }
    else if (NORMAL_COVERAGE> sigStrength && sigStrength>= ROBUST_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        preambleCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << packetMac->getType();
        EV <<" :  Packet no " << preambleCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);


        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;
    }

    else if (ROBUST_COVERAGE> sigStrength && sigStrength>= EXTREME_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        preambleCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << packetMac->getType();
        EV <<" :  Packet no " << preambleCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;
    }

    else {
        EV <<"UE is not in coverage area"<<endl;
    }
}






/************* Send the first random feedback message to trigger repetition ***************/

void Mac::RAFeedbackTransmit(simtime_t scheduleT, cMessage *msg){
    nbPacket *packetMac = check_and_cast<nbPacket *>(msg);

    if (sigStrength>= NORMAL_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        feedbackCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << packetMac->getType();
        EV <<" :  Packet no " << feedbackCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;
    }
    else if (NORMAL_COVERAGE> sigStrength && sigStrength>= ROBUST_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        feedbackCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << packetMac->getType();
        EV <<" :  Packet no " << feedbackCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;
    }
    else if (ROBUST_COVERAGE> sigStrength && sigStrength>= EXTREME_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        feedbackCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << packetMac->getType();
        EV <<" :  Packet no " << feedbackCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;
    }

    else {
        EV <<"UE is not in coverage area"<<endl;
    }
}





/********************* Repetition of RA preamble message *********************/

void Mac::RAPreambleRepetition(int repFreq, simtime_t scheduleT, cMessage *msg){
    if (preambleCounter<repFreq){
        nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        preambleCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << "RA_PREAMBLE";
        EV <<" : Packet no " << preambleCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;

    }
    else if(preambleCounter==repFreq){

        scheduleAt(simTime()+180, msg);     //RAR window time is 3 minutes
        EV<<"Starting Preamble RAR window";
        preambleCounter=repFreq+1;

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = RX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;
    }
    else{
        if (raResponseState == 0){
            nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
            EV<<"Starting Backoff";
            packetMac->setType(RA_BACKOFF);
            scheduleAt(simTime()+420, packetMac); //backoff time is 7 minutes
            preambleCounter = 0;

            cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
            calTime = simTime();
            powerLevel = IDLE_POWER_LEVEL;
            EV<<"New Power Level is " <<cumPower<<endl;
        }
        else{
            nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
            EV<<"Stopping sending preamble";
            packetMac->setType(RA_COMPLETE);
            scheduleAt(simTime(), packetMac);
            preambleCounter = 0;

            cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
            calTime = simTime();
            powerLevel = RX_POWER_LEVEL;   /////////Check
            EV<<"New Power Level is " <<cumPower<<endl;
        }
    }
}





/********************* Repetition of RA feedback message *********************/

void Mac::RAFeedbackRepetition(int repFreq, simtime_t scheduleT, cMessage *msg){
    if (feedbackCounter<repFreq){
        nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        feedbackCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << "RA_FEEDBACK";
        EV <<" : Packet no " << feedbackCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;

    }
    else if(feedbackCounter==repFreq){

        scheduleAt(simTime()+180, msg);     //RAR window time is 3 minutes

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = RX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;

        EV<<"Starting Feedback RAR window";
        feedbackCounter=repFreq+1;
    }
    else{
        if (contResState == 0){
            nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
            EV<<"Starting Backoff";
            packetMac->setType(RA_BACKOFF);
            scheduleAt(simTime()+420, packetMac); //backoff time is 7 minutes
            feedbackCounter = 0;

            cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
            calTime = simTime();
            powerLevel = IDLE_POWER_LEVEL;
            EV<<"New Power Level is " <<cumPower<<endl;
        }
        else {
            nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
            EV<<"Stopping sending Feedback" <<endl;
            packetMac->setType(RA_COMPLETE);
            scheduleAt(simTime(), packetMac);
            feedbackCounter = 0;

            cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
            calTime = simTime();
            powerLevel = RX_POWER_LEVEL;      /////////Check
            EV<<"New Power Level is " <<cumPower<<endl;
        }
    }
}









/************* Send the first SDU to trigger repetition ***************/

void Mac::SDUTransmit(simtime_t scheduleT, cMessage *msg){
    nbPacket *packetMac = check_and_cast<nbPacket *>(msg);

    if (sigStrength>= NORMAL_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        sduCounter++;
        EV << "MAC LowerGate Sending type MAC SDU";
        EV <<" :  Packet no " << sduCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);


        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;



    }
    else if (NORMAL_COVERAGE> sigStrength && sigStrength>= ROBUST_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        sduCounter++;
        EV << "MAC LowerGate Sending type MAC SDU";
        EV <<" :  Packet no " << sduCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);


        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;
    }

    else if (ROBUST_COVERAGE> sigStrength && sigStrength>= EXTREME_COVERAGE){

        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        sduCounter++;
        EV << "MAC LowerGate Sending type MAC SDU";
        EV <<" :  Packet no " << sduCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;
    }
    else {
        EV <<"Device is not in service area"<< endl;
//        emit(powerVector, cumPower);
        UEpower.record(cumPower);
    }
}




/********************* Repetition of SDU *********************/

void Mac::SDURepetition(int repFreq, simtime_t scheduleT, cMessage *msg){
    if (sduCounter<repFreq){
        nbPacket *packetMac = check_and_cast<nbPacket *>(msg);
        cMessage *copyPacket = packetMac->dup();
        send(copyPacket,"lowerGateOut");
        sduCounter++;
        EV << "MAC LowerGate Sending type ";
        EV << "MAC SDU";
        EV <<" : Packet no " << sduCounter << endl;
        scheduleAt(simTime()+scheduleT, packetMac);

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = TX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;

    }
    else{
        cancelAndDelete(msg);
        EV<<"Stop Sending SDU"<<endl;

        cumPower= cumPower - powerLevel*SIMTIME_DBL((simTime()-calTime));
        calTime = simTime();
        powerLevel = RX_POWER_LEVEL;
        EV<<"New Power Level is " <<cumPower<<endl;

        sduCounter = 0;
        UEpower.record(cumPower);

        packetdelay.record(simTime()-packetdelaycounter);   //Check this
        packetdelaycounter = 0;
    }
}



