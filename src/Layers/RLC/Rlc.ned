
package nbiot.Layers.RLC;

simple Rlc
{
    parameters:
        @display("i=block/transport"); // add a default icon
    gates:
        input lowerGateIn;
        output lowerGateOut;
        input upperGateIn;
        output upperGateOut;
}