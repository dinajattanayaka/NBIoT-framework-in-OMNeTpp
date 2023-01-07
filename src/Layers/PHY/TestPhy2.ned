package nbiot.Layers.PHY;

simple TestPhy2
{
    parameters:
        @display("i=device/antennatower;bgb=814,437"); // add a default icon
        int numNodes;
        
    gates:
        input testRadioIn @directIn;
}