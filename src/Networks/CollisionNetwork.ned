//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package nbiot.Networks;

import nbiot.Modules.*;
import nbiot.Layers.PHY.*;


network CollisionNetwork
{
    parameters:
        @display("bgb=1200,1200;bgi=background/terrain,s");
        int numNodes = default(1);


    submodules:
        UE[numNodes]: UE {
            @display("p=317,122");
        }
        eNodeB: TestPhy2 {
            @display("p=600,600");
        }
        ChannelController: ChannelController {
            @display("p=38,30");
        }

    connections:
}