

package nbiot.Layers.PHY;

simple ChannelController
{
	parameters:
	    @display("i=block/network2");
	    int netEnv = default(70);            //URBAN-70 , SUBURBAN-71, RURAL-72
	    
}