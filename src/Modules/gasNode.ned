
package nbiot.Modules;


import nbiot.Layers.APP.*;
import nbiot.Layers.RRC_PDCP.*;
import nbiot.Layers.MAC.*;
import nbiot.Layers.PHY.*;
import nbiot.Layers.RLC.*;


module gasMeter
{
    parameters:
    	@networkNode();
        @display("i=device/memory_vs");
        
    gates:
        input radioIn @directIn;     // connection to master
    
        
    submodules:
        App: App {
            @display("p=253,41");
        }
        Rrc: Rrc {
            @display("p=110,41");
        }
        Rlc: Rlc {
            @display("p=110,101");
        }
        Mac: Mac {
            @display("p=110,169");
        }


        Phy: Phy {

            @display("p=201,169");
        }


    connections:
        
        
        Mac.lowerGateOut --> {  delay = 0ms; } --> Phy.upperGateIn;
        Mac.lowerGateIn <-- {  delay = 0ms; } <-- Phy.upperGateOut;
        Mac.upperGateOut --> {  delay = 0ms; } --> Rlc.lowerGateIn;
        Mac.upperGateIn <-- {  delay = 0ms; } <-- Rlc.lowerGateOut;
        Rlc.upperGateOut --> {  delay = 0ms; } --> Rrc.lowerGateIn;
        Rlc.upperGateIn <-- {  delay = 0ms; } <-- Rrc.lowerGateOut;
        Rrc.upperGateOut --> {  delay = 0ms; } --> App.lowerGateIn;
        Rrc.upperGateIn <-- {  delay = 0ms; } <-- App.lowerGateOut;
        
        Phy.radioIn <-- radioIn;
        

}