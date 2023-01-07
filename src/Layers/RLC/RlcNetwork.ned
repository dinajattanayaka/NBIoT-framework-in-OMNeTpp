package nbiot.Layers.RLC;

import nbiot.Layers.RLC.Rlc;
import nbiot.Layers.RLC.TestRlc;



network RlcNetwork
{
    @display("bgb=395,278");
    submodules:
        Rlc: Rlc {
            parameters:
                @display("p=87,80");
        }
        TestRlc: TestRlc {
            parameters:
                @display("p=295,45;is=l");
        }
        TestRlc2: TestRlc {
            parameters:
                @display("p=268,192;is=l");
        }
    connections:
        Rlc.lowerGateOut --> {  delay = 100ms; } --> TestRlc2.in;
        Rlc.lowerGateIn <-- {  delay = 100ms; } <-- TestRlc2.out;
        Rlc.upperGateOut --> {  delay = 100ms; } --> TestRlc.in;
        Rlc.upperGateIn <-- {  delay = 100ms; } <-- TestRlc.out;

}
