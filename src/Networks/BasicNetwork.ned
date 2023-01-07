
package nbiot.Networks;

import nbiot.Modules.*;
import nbiot.Layers.PHY.*;


network BasicNetwork
{
    parameters:
        @display("bgb=850,1000;bgi=background/terrain,s");
        int numNodes = default(1);


    submodules:
        UE[numNodes]: UE {
            @display("p=317,122");
        }
        eNodeB: TestPhy2 {
            @display("p=200,150");
        }
        ChannelController: ChannelController {
            @display("p=38,30");
        }

    connections:
}
