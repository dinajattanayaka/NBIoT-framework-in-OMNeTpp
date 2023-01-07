
package nbiot.Layers.PHY;

simple Phy
{
    parameters:
        @display("i=block/wrxtx"); // add a default icon
        double x;
        double y;
        int trafficType = default(73);  // 73 - electric
        
    gates:
        input upperGateIn;
        output upperGateOut;
        input radioIn @directIn;
}