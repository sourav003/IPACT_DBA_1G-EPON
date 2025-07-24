/*
 * splitter.cc
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

class Splitter : public cSimpleModule
{
    private:
        cQueue onu_queue;            // Queue for packets to be sent to ONUs
        cQueue olt_queue;            // Queue for packets to be sent to OLT
        int onu_queue_size;
        int olt_queue_size;

        cMessage *olt_tx;
        cMessage *onu_tx;

    protected:
       // The following redefined virtual function holds the algorithm.
       virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Splitter);

void Splitter::initialize()
{
    onu_queue.setName("onu_queue");
    olt_queue.setName("olt_queue");
    olt_queue_size = 0;
    onu_queue_size = 0;
    // Make sure incoming message is delivered immediately
    gate("OltGate_i")->setDeliverImmediately(true);
    int n = gateSize("OnuGate_o");
    for (int k = 0; k < n; k++) {
        gate("OnuGate_i",k)->setDeliverImmediately(true);
    }
}

void Splitter::handleMessage(cMessage *msg)
{
    if(msg->isPacket() == true) {
        //EV << "[splt] Packet arrived to Splitter at "<< simTime() << endl;
        ponPacket *pkt = check_and_cast<ponPacket *>(msg);
        if(pkt->arrivedOn("OltGate_i") == true) {     // any packets arriving from OLT are broadcasted to all ONUs
            //EV << "arrivedOn " << pkt->arrivedOn("OltGate_i") << endl;
            int n = gateSize("OnuGate_o");
            for (int k = 0; k < n; k++) {
                //EV << "[splt] sending packet to ONU-"<< k <<" at "<< simTime() << endl;
                cGate *onu_gate = gate("OnuGate_o",k);
                cChannel *onu_ch = onu_gate->getChannel();
                if((onu_ch->isBusy() == false)&&(onu_queue.getLength() == 0)) {
                    ponPacket *copy = pkt->dup();       // creating a copy for all and sending immediately
                    send(copy,"OnuGate_o",k);
                }
                else {
                    if(pkt->getOnuId() == k) {            // if the channel for intended ONU is busy, then queue else drop
                        //EV << "[splt] channel busy so queuing for ONU "<< k <<" at "<< simTime() << endl;
                        onu_queue.insert(pkt);
                        //EV << "[splt] 82 Packet to be transmitted at "<< onu_ch->getTransmissionFinishTime()+(simtime_t)(onu_queue_size*8/pon_link_datarate) << endl;
                        cMessage *onu_tx = new cMessage("ONU_Tx_Delay");
                        scheduleAt(onu_ch->getTransmissionFinishTime()+(simtime_t)(onu_queue_size*8/pon_link_datarate),onu_tx);
                        onu_queue_size += pkt->getByteLength();
                    }
                }
            }
            delete pkt;
        }
        else {                                      // any packets arriving from any ONU are sent to the OLT
            cGate *olt_gate = gate("OltGate_o");
            cChannel *olt_ch = olt_gate->getChannel();
            if((olt_ch->isBusy() == false)&&(olt_queue.getLength() == 0)) {
                send(pkt,"OltGate_o");
                //EV << "[splt] 76 sending packet to OLT at "<< simTime() << endl;
            }
            else {
                EV << "[splt] channel busy so queuing for OLT at "<< simTime() << endl;
                olt_queue.insert(pkt);
                //EV << "[splt] 82 Packet to be transmitted at "<< olt_ch->getTransmissionFinishTime()+(simtime_t)(olt_queue_size*8/pon_link_datarate) << endl;
                cMessage *olt_tx = new cMessage("OLT_Tx_Delay");
                scheduleAt(olt_ch->getTransmissionFinishTime()+(simtime_t)(olt_queue_size*8/pon_link_datarate),olt_tx);
                olt_queue_size += pkt->getByteLength();
            }
        }
    }
    else {
        if(msg->isSelfMessage()) {
            if(strcmp(msg->getName(),"OLT_Tx_Delay") == 0) {
                //EV << "[splt] sending packet to OLT at "<< simTime() << endl;
                ponPacket *pkt = (ponPacket *)olt_queue.pop();
                send(pkt,"OltGate_o");
                olt_queue_size -= pkt->getByteLength();
                delete msg;
            }
            else if(strcmp(msg->getName(),"ONU_Tx_Delay") == 0) {
                ponPacket *pkt = (ponPacket *)onu_queue.pop();
                int id = pkt->getOnuId();
                send(pkt,"OnuGate_o",id);
                onu_queue_size -= pkt->getByteLength();
                //EV << "[splt] sending packet to ONU" << id << " at "<< simTime() << endl;
                delete msg;
            }
        }
        else {
            ping *pkt = check_and_cast<ping *>(msg);
            //EV << "arrivedOn " << pkt->arrivedOn("OltGate$i") << endl;
            if(pkt->arrivedOn("OltGate_i") == true) {     // any packets arriving from OLT are broadcasted to all ONUs
                //EV << "[splt] Ping at OltGate" << endl;
                int n = gateSize("OnuGate_o");
                //EV << "n = " << n << endl;
                for (int k = 0; k < n; k++) {
                    ping *copy = pkt->dup();       // creating a copy for all
                    send(copy,"OnuGate_o",k);
                    //EV << "[splt] Forwarding ping to ONU-" << k << endl;
                }
                delete pkt;
            }
            else {                                      // any packets arriving from any ONU are sent to the OLT
                send(pkt,"OltGate_o");
                //EV << "[splt] Forwarding ping to OLT" << endl;
            }
        }
    }
}










