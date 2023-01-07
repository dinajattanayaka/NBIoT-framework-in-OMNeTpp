

package nbiot.Layers.PHY;

simple TestBase
{
    parameters:
        @display("i=device/antennatower;bgb=814,437"); // add a default icon
        int numNodes;
        int gasNodes = default(0);
        int waterNodes = default(0);
		int vendingNodes = default(0);
		int fleetNodes = default(0);    //Bike fleet management
		int parkingNodes = default(0);
        
    gates:
        input testRadioIn @directIn;
}