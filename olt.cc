/*
 * olt.cc
 *
 *  Created on: 21 June 2024
 *      Author: MONDALS
 */

#include <string.h>
#include <math.h>
#include <omnetpp.h>
#include <numeric>   // Required for std::iota
#include <algorithm> // Required for std::sort

#include "sim_params.h"
#include "ponPacket_m.h"
#include "ping_m.h"

using namespace std;
using namespace omnetpp;

class OLT : public cSimpleModule
{
    private:
        //cQueue olt_queue;
        vector<double> onu_rtt;
        vector<double> onu_grants;
        vector<double> onu_total_latency;
        vector<double> onu_grant_times;
        vector<int> indices;
        int onus;
        int ping_count = 0;

        simsignal_t errorSignal;

    public:
        //virtual ~OLT();

    protected:
        double ber;
        long totalBitsReceived = 0;
        long totalPacketsReceived = 0;
        long corruptedPackets = 0;
        double onu_max_grant = 0;
        double max_polling_cycle = 0;

        // The following redefined virtual function holds the algorithm.
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        //virtual ponPacket *generateGrantPacket();
};

Define_Module(OLT);

void OLT::initialize()
{
    errorSignal = registerSignal("pkt_error");  // registering the signal
    //olt_queue.setName("olt_queue");

    onus = par("NumberOfONUs");
    EV << "[olt] No. of ONUs detected = " << onus << endl;

    max_polling_cycle = par("max_polling_cycle");
    onu_max_grant = floor((max_polling_cycle*1e-3 - T_guard*onus)*(pon_link_datarate/onus));

    for(int j = 0; j<onus; j++) {
        onu_rtt.push_back(0);
        onu_grants.push_back(onu_max_grant);        // allocating max grant at the beginning
        onu_total_latency.push_back(0);
        onu_grant_times.push_back(0);
        indices.push_back(0);
    }
    //EV << "[olt] onu_rtt[0] = " << onu_rtt[0] << ", onu_rtt[1] = " << onu_rtt[1] << endl;

    ping *png = new ping("ping");      // sending ping message at T = 0 for finding the RTT of all ONUs
    send(png,"SpltGate_o");
    //EV << "[olt] Sending ping from OLT" << endl;
}

void OLT::handleMessage(cMessage *msg)
{
    if(msg->isPacket() == true) {
        ponPacket *pkt = check_and_cast<ponPacket *>(msg);

        if(strcmp(pkt->getName(),"app_data") == 0) {
            //olt_queue.insert(pkt);
            //EV <<"[olt] Packet arrival time = " << simTime() << endl;

            /* implementing packet error */
            ber = par("ber").doubleValue();

            int pktSizeBits = pkt->getByteLength() * 8;
            totalBitsReceived += pktSizeBits;
            totalPacketsReceived++;

            // PER ≈ 1 - (1 - BER)^N
            double per = 1 - pow(1 - ber, pktSizeBits);
            double r = uniform(0, 1);

            if (r < per) {
                corruptedPackets++;
                EV << "Packet corrupted due to BER\n";
                emit(errorSignal,corruptedPackets);
            }

            delete pkt;
        }
        else if(strcmp(pkt->getName(),"RequestONU") == 0) {
            int ind = pkt->getOnuId();
            onu_grants[ind] = min(pkt->getRequest(),onu_max_grant);
            //onu_grants[ind] = onu_max_grant;
            onu_total_latency[ind] = onu_rtt[ind] + (onu_grants[ind]*8)/pon_link_datarate + T_guard;
            //EV << "Received request. Next grant = " << onu_grants[ind] << " onu_total_latency = " << onu_total_latency[ind] << endl;
            delete pkt;
        }
    }
    else {
        if(strcmp(msg->getName(),"ping") == 0) {
            ping_count += 1;
            ping *png = check_and_cast<ping *>(msg);
            int onu_id = png->getONU_id();
            onu_rtt[onu_id] = png->getArrivalTime().dbl();
            onu_total_latency[onu_id] = onu_rtt[onu_id] + (onu_grants[onu_id]*8)/pon_link_datarate + T_guard;

            //EV << "[olt] Received ping response from ONU-" << onu_id << " and RTT = " << onu_rtt[onu_id] << endl;
            //EV << "[olt] onu_rtt[0] = " << onu_rtt[0] << ", onu_rtt[1] = " << onu_rtt[1] << endl;
            //EV << "[olt] onu_rtt[2] = " << onu_rtt[2] << ", onu_rtt[3] = " << onu_rtt[3] << endl;

            if(ping_count == onus) {
                //EV << "[olt] onu_total_latency[0] = " << onu_total_latency[0] << ", onu_total_latency[1] = " << onu_total_latency[1] << endl;
                cMessage *GrantSchedule = new cMessage("GrantSchedule");
                scheduleAt(simTime(), GrantSchedule);           // when ping from all ONUs arrive, initiate the grant scheduling process
            }
            delete png;
        }
        else if(strcmp(msg->getName(),"GrantSchedule") == 0) {  // calculating the time-instants for sending grants to onus
            cancelAndDelete(msg);

            double olt_time = simTime().dbl();                  // starting from current time instant

            // Fill with 0,1,...,N-1
            std::iota(indices.begin(), indices.end(), 0);
            std::sort(indices.begin(), indices.end(),
                    [&](int i1, int i2) {
                        return onu_total_latency[i1] < onu_total_latency[i2];
                    });
            // You can also see the indices themselves:
            EV << "Sorted indices: ";
            for (int i : indices) {
                EV << i << " ";
            }
            EV << endl;

            for(int i = 0; i < onus; i++) {
                if(i == 0){
                    onu_grant_times[indices[i]] = olt_time;
                }
                else {
                    onu_grant_times[indices[i]] = olt_time + onu_total_latency[indices[i-1]] + (grant_reqst_size*8/pon_link_datarate) - onu_rtt[indices[i]];
                    olt_time = onu_grant_times[indices[i]];              // shifting the time cursor
                }
                EV << "[olt] onu_grant_times[" << indices[i] << "] = "<< onu_grant_times[indices[i]] << endl;
            }
            //EV << "[olt] onu_grant_times[0] = " << onu_grant_times[0] << " onu_grant_times[1] = "<< onu_grant_times[1]<< endl;
            //EV << "[olt] onu_grant_times[2] = " << onu_grant_times[2] << " onu_grant_times[3] = "<< onu_grant_times[3]<< endl;

            cMessage *sendGrants = new cMessage("sendGrants");
            scheduleAt((simtime_t)onu_grant_times[indices[0]], sendGrants);
        }
        else if(strcmp(msg->getName(),"sendGrants") == 0) {
            EV << "[olt] ping_count = " << ping_count << ", onus = " << onus <<endl;
            int id = ping_count % onus;
            EV << "[olt] Sending grant to ONU = " << indices[id] << " at: " << simTime() <<endl;
            ponPacket *grant = new ponPacket("GrantONU");
            grant->setByteLength(grant_reqst_size);
            grant->setIsGrant(true);
            grant->setOnuId(indices[id]);
            grant->setGrant(onu_grants[indices[id]]);
            ping_count += 1;                    // re-using the same variable instead of defining a new one
            EV << "[olt] ping_count = " << ping_count << endl;
            send(grant,"SpltGate_o");

            if(id < onus-1)
                scheduleAt((simtime_t)(onu_grant_times[indices[id+1]]), msg);
            else if(id == onus-1) {
                cancelAndDelete(msg);
                cMessage *GrantSchedule = new cMessage("GrantSchedule");
                scheduleAt((simtime_t)(onu_grant_times[indices[id]]+onu_total_latency[indices[id]]), GrantSchedule);
            }
        }
    }
}









