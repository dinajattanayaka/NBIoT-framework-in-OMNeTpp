
package nbiot.Layers.MAC;

simple Mac
{
    parameters:
//        @signal[UEpower](type="double");
//	    @statistic[batteryPower](title="Battery power of UE"; source="UEpower"; record=vector,stats; interpolationmode=none);
	
        @display("i=block/mac"); // add a default icon
    gates:
        input lowerGateIn;
        output lowerGateOut;
        input upperGateIn;
        output upperGateOut;
}