

package nbiot.Layers.MAC;




network MacNetwork
{
    @display("bgb=395,278");
    submodules:
        Mac: Mac {
            parameters:
                @display("p=87,80;is=l");
        }
        TestMac: TestMac {
            parameters:
                @display("p=309,183");
        }
        TestMac2: TestMac2 {
            parameters:
                @display("p=309,37");
        }
    connections:
        Mac.lowerGateOut --> {  delay = 0ms; } --> TestMac2.in;
        Mac.lowerGateIn <-- {  delay = 0ms; } <-- TestMac2.out;
        Mac.upperGateOut --> {  delay = 0ms; } --> TestMac.in;
        Mac.upperGateIn <-- {  delay = 0ms; } <-- TestMac.out;

}
