
package nbiot.Layers.PHY;

network PhyNetwork
{
    @display("bgb=395,278");
    submodules:
        Phy: Phy {
            parameters:
                @display("p=167,146;is=l");
        }
        TestPhy: TestPhy {
            parameters:
                @display("p=300,98");
        }
        TestPhy2: TestPhy2 {
            parameters:
                @display("p=300,194");
        }

        channelController: ChannelController {
            @display("p=42,27");
        }
    connections:

        Phy.upperGateOut --> {  delay = 0.1ms; } --> TestPhy.in;
        Phy.upperGateIn <-- {  delay = 0.1ms; } <-- TestPhy.out;

}
