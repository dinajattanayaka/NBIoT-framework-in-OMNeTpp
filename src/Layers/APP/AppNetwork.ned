package nbiot.Layers.APP;

import nbiot.Layers.APP.App;
import nbiot.Layers.APP.TestApp;


network AppNetwork
{
    @display("bgb=395,278");
    submodules:
        App: App {
            parameters:
                @display("i=,cyan;p=87,80");
        }
        TestApp: TestApp {
            parameters:
                @display("i=,gold;p=87,199");
        }

    connections:
        App.lowerGateOut --> {  delay = 100ms; } --> TestApp.in;
        App.lowerGateIn <-- {  delay = 100ms; } <-- TestApp.out;
}
