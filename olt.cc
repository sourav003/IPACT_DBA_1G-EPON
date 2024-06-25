/*
 * olt.cc
 *
 *  Created on: 21 June 2024
 *      Author: MONDALS
 */

#include <string.h>
#include <math.h>
#include <omnetpp.h>

#include "sim_params.h"
#include "ponPacket_m.h"
#include "ping_m.h"

using namespace std;
using namespace omnetpp;

class OLT : public cSimpleModule
{
    private:
        cQueue olt_queue;
        vector<double> onu_rtt;
        vector<double> onu_grants;
        vector<double> onu_total_latency;
        vector<double> onu_grant_times;
        int onus;
        int ping_count = 0;
        int rqst_count = 0;

    public:
        //virtual ~OLT();

    protected:
        // The following redefined virtual function holds the algorithm.
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        //virtual ponPacket *generateGrantPacket();
};

Define_Module(OLT);

void OLT::initialize()
{
    olt_queue.setName("olt_queue");

    onus = par("NumberOfONUs");
    EV << "[olt] No. of ONUs detected = " << onus << endl;

    for(int j = 0; j<onus; j++) {
        onu_rtt.push_back(0);
        onu_grants.push_back(onu_max_grant);        // allocating max grant at the beginning
        onu_total_latency.push_back(0);
        onu_grant_times.push_back(0);
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
            olt_queue.insert(pkt);
            EV <<"[olt] Packet arrival time = " << simTime() << endl;
        }
        else if(strcmp(pkt->getName(),"RequestONU") == 0) {
            int ind = pkt->getOnuId();
            onu_grants[ind] = min(pkt->getRequest(),onu_max_grant);
            onu_total_latency[ind] = onu_rtt[ind] + (onu_grants[ind]*8)/pon_link_datarate;
            EV << "Received request. Next grant = " << onu_grants[ind] << " onu_total_latency = " << onu_total_latency[ind] << endl;
            delete pkt;

            rqst_count += 1;
            rqst_count %= onus;
            if(rqst_count == 0) {   // when requests from all ONUs are received

            }
        }
    }
    else {
        if(strcmp(msg->getName(),"ping") == 0) {
            ping_count += 1;
            ping *png = check_and_cast<ping *>(msg);
            int onu_id = png->getONU_id();
            onu_rtt[onu_id] = png->getArrivalTime().dbl();
            onu_total_latency[onu_id] = onu_rtt[onu_id] + (onu_grants[onu_id]*8)/pon_link_datarate;

            //EV << "[olt] Received ping response from ONU-" << onu_id << " and RTT = " << onu_rtt[onu_id] << endl;
            EV << "[olt] onu_rtt[0] = " << onu_rtt[0] << ", onu_rtt[1] = " << onu_rtt[1] << endl;
            EV << "[olt] onu_rtt[2] = " << onu_rtt[2] << ", onu_rtt[3] = " << onu_rtt[3] << endl;

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
            for(int i = 0; i < onus; i++) {
                if(i == 0){
                    onu_grant_times[i] = olt_time;
                }
                else {
                    onu_grant_times[i] = olt_time + onu_total_latency[i-1] + T_guard + (grant_reqst_size*8/pon_link_datarate) - onu_rtt[i];
                    olt_time = onu_grant_times[i];              // shifting the time cursor
                }
            }
            EV << "[olt] onu_grant_times[0] = " << onu_grant_times[0] << " onu_grant_times[1] = "<< onu_grant_times[1]<< endl;
            EV << "[olt] onu_grant_times[2] = " << onu_grant_times[2] << " onu_grant_times[3] = "<< onu_grant_times[3]<< endl;

            cMessage *sendGrants = new cMessage("sendGrants");
            scheduleAt((simtime_t)onu_grant_times[0], sendGrants);
        }
        else if(strcmp(msg->getName(),"sendGrants") == 0) {
            int id = ping_count % onus;
            EV << "[olt] Sending grant to ONU = " << id << " at: " << simTime() <<endl;
            ponPacket *grant = new ponPacket("GrantONU");
            grant->setByteLength(4);
            grant->setIsGrant(true);
            grant->setOnuId(id);
            grant->setGrant(onu_grants[id]);
            ping_count += 1;                    // re-using the same variable instead of defining a new one
            send(grant,"SpltGate_o");

            if(id < onus-1)
                scheduleAt((simtime_t)(onu_grant_times[id+1]), msg);
            else if(id == onus-1) {
                cancelAndDelete(msg);
                cMessage *GrantSchedule = new cMessage("GrantSchedule");
                scheduleAt((simtime_t)(onu_grant_times[id]+onu_total_latency[id]+T_guard), GrantSchedule);
            }
        }
    }
}









