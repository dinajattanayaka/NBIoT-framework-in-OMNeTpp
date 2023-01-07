
package nbiot.Layers.PDCP;

import nbiot.Layers.PDCP.Pdcp;
import nbiot.Layers.PDCP.TestPdcp;


network PdcpNetwork
{
    @display("bgb=395,278");
    submodules:
        Pdcp: Pdcp {
            parameters:
                @display("i=,cyan;p=87,80");
        }
        TestPdcp: TestPdcp {
            parameters:
                @display("i=,gold;p=295,45;is=l");
        }
        TestPdcp2: TestPdcp {
            parameters:
                @display("i=,gold;p=268,192;is=l");
        }
    connections:
        Pdcp.lowerGateOut --> {  delay = 100ms; } --> TestPdcp2.in;
        Pdcp.lowerGateIn <-- {  delay = 100ms; } <-- TestPdcp2.out;
        Pdcp.upperGateOut --> {  delay = 100ms; } --> TestPdcp.in;
        Pdcp.upperGateIn <-- {  delay = 100ms; } <-- TestPdcp.out;

}
