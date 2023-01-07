cplusplus {{
    #include "packetDefinition.h"
}}

packet nbPacket {
    int type ;
    int schedulingFrequency = 0;
    double signalStrength;   //for coverage levels
    double carrierFreq;
    int ueID;
    int RAChannelID;
    int uplinkChannelID;
    int downlinkChannelID;
    
    int trafficType;
    
    double preambleVal;
}
