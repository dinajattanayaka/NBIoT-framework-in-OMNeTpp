
package nbiot.Networks;

import nbiot.Modules.*;
import nbiot.Layers.PHY.*;


network CongestedNetwork
{
    parameters:
        @display("bgb=2060,2060;bgi=background/terrain,s");
        int numNodes = default(1);


    submodules:
        UE[numNodes]: UE {
            @display("p=1725,1725");
        }
        eNodeB: TestPhy2 {
            @display("p=1030,1030");
        }
        ChannelController: ChannelController {
            @display("p=38,30");
        }

    connections:
}
