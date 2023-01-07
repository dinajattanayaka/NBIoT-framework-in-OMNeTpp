
package nbiot.Layers.basicNetwork;

import nbiot.Layers.RRC_PDCP.Rrc;
import nbiot.Layers.RRC_PDCP.TestRrc;
import nbiot.Layers.RLC.Rlc;
import nbiot.Layers.MAC.Mac;
import nbiot.Layers.APP.App;
import nbiot.Layers.PHY.Phy;
import nbiot.Layers.PHY.ChannelController;
import nbiot.Layers.PHY.TestPhy2;


network basicNetwork
{
    @display("bgb=348,224");
    submodules:

        App: App {
            @display("p=253,41");
        }
        Rrc: Rrc {
            @display("p=110,41");
        }
        Rlc: Rlc {
            @display("p=110,101");
        }
        Mac: Mac {
            @display("p=110,169");
        }


        Phy: Phy {

            @display("p=201,169");
        }

        TestPhy2: TestPhy2 {

            @display("p=292,169");
        }

        ChannelController: ChannelController {
            @display("p=35,35");
        }

    connections:
        Mac.lowerGateOut --> {  delay = 0ms; } --> Phy.upperGateIn;
        Mac.lowerGateIn <-- {  delay = 0ms; } <-- Phy.upperGateOut;
        Mac.upperGateOut --> {  delay = 0ms; } --> Rlc.lowerGateIn;
        Mac.upperGateIn <-- {  delay = 0ms; } <-- Rlc.lowerGateOut;
        Rlc.upperGateOut --> {  delay = 0ms; } --> Rrc.lowerGateIn;
        Rlc.upperGateIn <-- {  delay = 0ms; } <-- Rrc.lowerGateOut;
        Rrc.upperGateOut --> {  delay = 0ms; } --> App.lowerGateIn;
        Rrc.upperGateIn <-- {  delay = 0ms; } <-- App.lowerGateOut;


}


