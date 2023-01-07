
package nbiot.Layers.RRC_PDCP;

simple Rrc
{
    parameters:
        @display("i=block/fork"); // add a default icon
    gates:
        input lowerGateIn;
        output lowerGateOut;
        input upperGateIn;
        output upperGateOut;
}