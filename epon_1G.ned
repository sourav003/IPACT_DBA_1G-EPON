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

simple SourceApp
{
    parameters:
        @display("i=device/pc");
        double load = default(0.5);				// this will vary as 0.05:0.05:1 
        double dataRate = default(100e6);		// max datarate to ONU = 100 Mbps

        //@signal[generation](type="double");
        //@statistic[pkt_interval](title="packet inter-arrival time"; source="generation"; record=vector,stats; interpolationmode=none);
    gates:
        output out;
}

simple ONU
{
    parameters:
        @signal[latency](type="long");
        @statistic[packet_latency](title="packet latency at ONU"; source="latency"; record=vector,stats; interpolationmode=none);
        
        @display("i=device/smallrouter");
        
    gates:
        input inSrc;
        input SpltGate_i;
        output SpltGate_o;
}

simple OLT
{
    parameters:
        @display("i=device/router");
        int NumberOfONUs = default(2);
    gates:
        input SpltGate_i;
        output SpltGate_o;
}

simple Splitter
{
    parameters:
        @display("i=block/fork");
    gates:
        input OltGate_i;
        output OltGate_o;
        input OnuGate_i[];
        output OnuGate_o[];
}

network epon_1G
{
    types:
        channel PON_Channel extends ned.DatarateChannel
        {
            volatile double distance @unit(km) = default(10 km);
            delay = this.distance/(2e5 km)*1s;					// considering speed of light in fiber = 2x10^5 km/s
            datarate = 1 Gbps;
        }
        
    submodules:
        olt: OLT {
            @display("p=36,84");
        }
        splitter: Splitter {
            @display("p=153,84");
        }
        onus[olt.NumberOfONUs]: ONU {
            @display("p=389,37,c");
        }
        sources[olt.NumberOfONUs]: SourceApp {
            @display("p=498,38,c");
        }
        
    connections:
        // OLT-Splitter connections
        olt.SpltGate_o --> PON_Channel --> splitter.OltGate_i;
        olt.SpltGate_i <-- PON_Channel <-- splitter.OltGate_o;
        for i=0..(olt.NumberOfONUs-1) {
            splitter.OnuGate_o++ --> PON_Channel --> onus[i].SpltGate_i;		// ONU-Splitter connections
            //splitter.OnuGate++ <--> PON_Channel{distance = uniform(5km,10km);} <--> onus[i].SpltGate;
            splitter.OnuGate_i++ <-- PON_Channel <-- onus[i].SpltGate_o;
            onus[i].inSrc <-- sources[i].out; 								// App-ONU connections
        }
}











