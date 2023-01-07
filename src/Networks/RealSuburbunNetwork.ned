
package nbiot.Networks;

import nbiot.Modules.*;
import nbiot.Layers.PHY.*;


network RealSuburbunNetwork
{
    parameters:
        //        @display("bgb=500,500;bgi=background/monaragala");
        @display("bgi=background/suburban,s;bgb=1000,1000");

        int numNodes = default(1);


    submodules:
        UE[numNodes]: UE {
            @display("p=317,122");
        }
        eNodeB: TestPhy2 {
            @display("p=500,500");
        }
        ChannelController: ChannelController {
            @display("p=38,30");
        }

    connections:
}
