
package nbiot.Layers.PDCP;

simple Pdcp
{
    parameters:
        @display("i=block/fork"); // add a default icon
    gates:
        input lowerGateIn;
        output lowerGateOut;
        input upperGateIn;
        output upperGateOut;
}