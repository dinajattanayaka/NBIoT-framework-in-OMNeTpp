
package nbiot.Networks;

import nbiot.Modules.*;
import nbiot.Layers.PHY.*;


network MultipleTrafficNetwork
{
    parameters:
        @display("bgb=1000,1000;bgi=background/terrain,s");
        int numNodes = default(1);
        int gasNodes = default(0);
        int waterNodes = default(0);
		int vendingNodes = default(0);
		int fleetNodes = default(0);    //Bike fleet management
		int parkingNodes = default(0);
		

    submodules:
        UE[numNodes]: UE {
            @display("p=317,122");
        }
        eNodeB: TestBase {
            @display("p=0,0");
        }
        ChannelController: ChannelController {
            @display("p=38,30");
        }
        
        Gas[gasNodes] : gasMeter{
            
        }
        Water[waterNodes]:waterMeter{
        }
        
        Vending[vendingNodes]:VendingNode{
        }
        
        Fleet[fleetNodes]:fleetNode{
        }
        
        Park[parkingNodes]:parkingNode{
        }

    connections:
}
