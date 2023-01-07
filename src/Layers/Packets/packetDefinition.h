
#ifndef LAYERS_PACKETS_PACKETDEFINITION_H_
#define LAYERS_PACKETS_PACKETDEFINITION_H_


#include <iostream>
#include <omnetpp.h>
#include <string>

using namespace omnetpp;


#define CONNECTION_SETUP_REQUEST 0
#define CONNECTION_SETUP 1
#define CONNECTION_SETUP_COMPLETE 2
#define CONNECTION_RESUME_REQUEST 3
#define CONNECTION_RESUME_COMPLETE 4
#define RRC_SDU 5
#define RRC_PDU 6
#define RRC_IDLE 7
#define RRC_CONNECTED 8
#define MAC_SDU 9
#define MAC_PDU 10
#define NET_SDU 11
#define NET_PDU 12
#define PHY_SDU 13
#define PHY_PDU 14
#define RLC_SDU 15
#define RLC_PDU 16

#define RA_PREAMBLE 17
#define RA_RESPONSE 18
#define RA_FEEDBACK 19
#define RA_CONTENSION_RESOLUTION 20
#define RA_BACKOFF 21               //self info to start backoff
#define RA_COMPLETE 22              //self info to go to feedback
#define RA_CONTENTION_MISMATCH 23

#define SIB2 24
#define RA_MISMATCH_RECOVER 25


#define TAU 30
#define IDLE1 31
#define IDLE2 32
#define IDLE3 33
#define PSM_TRIG 34
#define CONNECTION_RELEASE 35


#define NPSS 40
#define NPBCH_TIMER 41   // self message to set the timer to NPBCH
#define NPBCH_INITIAL 42 //state from NPSS receiving to NPBCH
#define NPCCH_INITIAL 43 //state from NPSS receiving to NPCCH
#define NPSCH_INITIAL 44 //state from NPSS receiving to NPCSCH
#define NPBCH_IDLE 45    //state just before NPBCH
#define UNKNOWN 46       //default type
#define PHY_IDLE 47
#define PHY_TRIGGER 48
#define PREAMBLE_SENT 49
#define NPCCH_TIMER 50
#define NPCCH_RA_RESPONSE_IDLE 51
#define NPCCH_RA_FEEDBACK_IDLE 52
#define RA_FEEDBACK_SENT 53
#define NPCCH_CONRES_IDLE 54
#define NPUSCH_IDLE 55
#define RA_CONTROLLER_TIMER 56
#define FEEDBACK_CONTROLLER_TIMER 57
#define PHY_DATA_SENT 58
#define CON_RELEASE_IDLE 59


#define ENB_NPSS_TIMER 60
#define ENB_PREAMBLE_TIMER 61
#define ENB_FEEDBACK_TIMER 62
#define ENB_UPLINK_TIMER 63
#define ENB_CON_RELEASE_TIMER 64
#define UE_TRAFFIC_TIMER 65


#define URBAN 70
#define SUBURBAN 71
#define RURAL 72

#define ELECTRIC 73
#define GAS 74
#define WATER 75
#define VENDING 76
#define FLEET 77
#define PARKING 78





//#define BATTERY_CAPACITY 58320     //battery capacity = 16.2Ah
//#define TX_POWER_LEVEL 0.175
//#define RX_POWER_LEVEL 0.065
//#define IDLE_POWER_LEVEL 0.002
//#define PSM_POWER_LEVEL 0.0000036

#define BATTERY_CAPACITY 18000000     //battery capacity = 5Wh
#define TX_POWER_LEVEL 545
#define RX_POWER_LEVEL 90
#define IDLE_POWER_LEVEL 3
#define PSM_POWER_LEVEL 0.015




#define NORMAL_COVERAGE -109
#define ROBUST_COVERAGE -119
#define EXTREME_COVERAGE -129

#define eNODEB_SENSITIVITY -141


//#define UE_POWER_RX 90        //min 90
//#define UE_POWER_TX_LOW 100
//#define UE_POWER_TX_MEDIUM 240
//#define UE_POWER_TX_HIGH   550 //max 565
////#define UE_POWER_IDLE 3
//#define UE_POWER_PSM 0.015




#endif /* LAYERS_PACKETS_PACKETDEFINITION_H_ */
