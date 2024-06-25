/*
 * source.cc
 *
 *  Created on: 22 June 2024
 *      Author: MONDALS
 */

#include <string.h>
#include <math.h>
#include <omnetpp.h>

#include "ponPacket_m.h"
#include "ping_m.h"
#include "sim_params.h"


using namespace std;
using namespace omnetpp;

/*
 * Each source generates packets following some probability distribution.
 * The average inter-arrival time is derived using the maximum datarate of the
 * SourceApp and average packet size.
 * The generateEvent is a self-message which is scheduled back-to-back to create
 * a new packet. Immediately the packet is transmitted to the corresponding ONU.
 */

class SourceApp : public cSimpleModule
{
    private:
        double Load;
        double ArrivalRate;
        double pkt_interval;                     // inter-packet generation interval
        cMessage *generateEvent = nullptr;       // holds pointer to the self-timeout message

        //simsignal_t arrivalSignal;               // to send signals for statistics collection

    public:
        virtual ~SourceApp();

    protected:
        // The following redefined virtual function holds the algorithm.
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual ponPacket *generateNewPacket();
};

// The module class needs to be registered with OMNeT++
Define_Module(SourceApp);

SourceApp::~SourceApp()
{
    cancelAndDelete(generateEvent);
}

void SourceApp::initialize()
{
    //arrivalSignal = registerSignal("generation");              // registering the signal

    Load = par("load");                                         // get the load factor from NED file
    double R_o = par("dataRate");                               // get the max ONU datarate from NED file
    ArrivalRate = Load*R_o/(8*pkt_sz_avg);                      // average packet arrival rate with datarate in bytes

    // Initialize variables
    pkt_interval = exponential(1/ArrivalRate);                  // packet inter-arrival times are generated following exponential distribution
    generateEvent = new cMessage("generateEvent");              // self-message is generated for next packet generation
    //emit(arrivalSignal,pkt_interval);

    ponPacket *pkt = generateNewPacket();                       // generating the first packet at T = 0
    send(pkt,"out");
    //EV << "[src] pkt_interval = " << pkt_interval << " and current time = " << simTime() << endl;

    scheduleAt(simTime()+pkt_interval, generateEvent);          // scheduling the next packet generation
}

void SourceApp::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getName(),"generateEvent") == 0) {
        cPacket *pkt = generateNewPacket();                     // generating a new packet at current time
        send(pkt,"out");

        pkt_interval = exponential(1/ArrivalRate);              // packet inter-arrival time generation
        scheduleAt(simTime()+pkt_interval, generateEvent);      // scheduling the next packet generation
        //emit(arrivalSignal,pkt_interval);
        //EV << "[src] pkt_interval = " << pkt_interval << " and current time = " << simTime() << endl;
    }
}

ponPacket *SourceApp::generateNewPacket()
{
    int pkt_size = intuniform(64,1542);
    ponPacket *pkt = new ponPacket("app_data");
    pkt->setByteLength(pkt_size);                   // adding a random size payload to the packet
    pkt->setGenerationTime(simTime());
    //EV << "[src] New packet generated with size (bytes): " << pkt_size << endl;
    return pkt;
}






