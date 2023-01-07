
package nbiot.Layers.RRC_PDCP;

import nbiot.Layers.RRC_PDCP.Rrc;
import nbiot.Layers.RRC_PDCP.TestRrc;


network RrcPdcpNetwork
{
    @display("bgb=395,278");
    submodules:
        Rrc: Rrc {
            parameters:
                @display("i=,cyan;p=87,80");
        }
        TestRrc: TestRrc {
            parameters:
                @display("i=,gold;p=295,45;is=l");
        }
        TestRrc2: TestRrc {
            parameters:
                @display("i=,gold;p=268,192;is=l");
        }
    connections:
        Rrc.lowerGateOut --> {  delay = 100ms; } --> TestRrc2.in;
        Rrc.lowerGateIn <-- {  delay = 100ms; } <-- TestRrc2.out;
        Rrc.upperGateOut --> {  delay = 100ms; } --> TestRrc.in;
        Rrc.upperGateIn <-- {  delay = 100ms; } <-- TestRrc.out;

}
