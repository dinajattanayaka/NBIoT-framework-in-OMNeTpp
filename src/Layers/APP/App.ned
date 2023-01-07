
package nbiot.Layers.APP;

simple App
{
    parameters:
        @display("i=block/app"); // add a default icon
        double interTransmitTime;
    gates:
        input lowerGateIn;
        output lowerGateOut;
}
