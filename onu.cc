/*
 * ONU.cc
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

class ONU : public cSimpleModule
{
    private:
        cQueue onu_queue;
        double capacity;                        // buffer size = 10 MB
        double pending_buffer;                  // pending data size in buffer
        double packet_drop_count;
        double current_grant;

        simsignal_t latencySignal;

    protected:
        // The following redefined virtual function holds the algorithm.
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(ONU);

void ONU::initialize()
{
    latencySignal = registerSignal("latency");  // registering the signal

    onu_queue.setName("onu_queue");
    capacity = onu_buffer_capacity;
    pending_buffer = 0;
    packet_drop_count = 0;
}

void ONU::handleMessage(cMessage *msg)
{
    if(msg->isPacket() == true) {
        ponPacket *pkt = check_and_cast<ponPacket *>(msg);

        if(pkt->arrivedOn("inSrc") == true) {
            double buffer = pending_buffer + pkt->getByteLength();      // future buffer size if current packet is queued
            if(buffer <= onu_buffer_capacity) {                         // queue the current packet if there is buffer capacity
                pkt->setOnuArrivalTime(simTime());
                pkt->setOnuId(getIndex());
                //EV << "[onu" << getIndex() << "] Arrived packet from source." << endl;
                onu_queue.insert(pkt);
                pending_buffer += pkt->getByteLength();
                //EV << "[onu" << getIndex() << "] Current queue length = " << pending_buffer << " at ONU = " << getIndex() <<endl;;
            }
            else {
                delete pkt;                     // buffer full so deleting the current packet
                packet_drop_count += 1;
            }
        }
        else if(pkt->arrivedOn("SpltGate_i") == true) {
            if((pkt->getIsGrant() == true)&&(pkt->getOnuId() != getIndex())) {
                delete pkt;
            }
            else if((pkt->getIsGrant() == true)&&(pkt->getOnuId() == getIndex())) {
                current_grant = pkt->getGrant();        // update the current grant value
                //EV << "[onu" << getIndex() << "] Received grant = " << current_grant << " at ONU = " << getIndex() <<endl;
                cMessage *packetSend = new cMessage("packetSend");
                scheduleAt(simTime(),packetSend);
                delete pkt;
            }
        }
    }
    else {      // if not packet but a message
        if(strcmp(msg->getName(),"ping") == 0) {
            ping *png = check_and_cast<ping *>(msg);
            png->setONU_id(getIndex());
            send(msg,"SpltGate_o");                   // immediately send the ping message back
            //EV << "[onu" << getIndex() << "] Sending ping response from ONU-" << getIndex() << endl;
        }
        else if(strcmp(msg->getName(),"packetSend") == 0) {
            if((current_grant > 0)&&(pending_buffer > 0)) {
                ponPacket *front = (ponPacket *)onu_queue.front();
                if(front->getByteLength() <= current_grant) {                // check if the first packet can be sent now
                    ponPacket *data = (ponPacket *)onu_queue.pop();          // pop and send the packet
                    current_grant -= data->getByteLength();
                    pending_buffer -= data->getByteLength();
                    send(data,"SpltGate_o");
                    data->setOnuDepartureTime(data->getSendingTime());
                    //EV << "[onu" << getIndex() << "] Packet SendingTime = " << data->getOnuDepartureTime() << endl;

                    simtime_t Txtime = (simtime_t)(data->getBitLength()/pon_link_datarate);               // multiplying with 2 because an extra (bitlength/linkrate) time will be consumed at splitter
                    scheduleAt(data->getSendingTime()+Txtime,msg);                  // re-scheduling packetSend
                    //EV << "[onu" << getIndex() << "] Packet sent length = " << data->getByteLength() << " current_grant: " << current_grant << " pending_buffer:" << pending_buffer << " at ONU = " << getIndex() <<endl;

                    double packet_latency = data->getOnuDepartureTime().dbl() - data->getOnuArrivalTime().dbl();
                    emit(latencySignal,packet_latency);
                }
                else {      // if the remaining grant is insufficient to send the next packet
                    //EV <<  "[onu" << getIndex() << "] current_grant: " << current_grant << " pending_buffer:" << pending_buffer << " at ONU = " << getIndex() <<endl;
                    ponPacket *request = new ponPacket("RequestONU");
                    request->setByteLength(grant_reqst_size);
                    request->setIsRequest(true);
                    request->setOnuId(getIndex());
                    request->setRequest(pending_buffer);
                    send(request,"SpltGate_o");
                    //EV << "[onu" << getIndex() << "] sending request = " << pending_buffer <<endl;
                    cancelAndDelete(msg);   // cleaning up packetSend msg
                }
            }
            else {      // either grant <= 0 or pending_buffer = 0
                //EV << "[onu" << getIndex() << "] current_grant: " << current_grant << " pending_buffer:" << pending_buffer << " at ONU = " << getIndex() <<endl;
                ponPacket *request = new ponPacket("RequestONU");
                request->setByteLength(grant_reqst_size);
                request->setIsRequest(true);
                request->setOnuId(getIndex());
                request->setRequest(pending_buffer);
                send(request,"SpltGate_o");
                //EV << "[onu" << getIndex() << "] sending request = " << pending_buffer << endl;
                cancelAndDelete(msg);       // cleaning up packetSend msg
            }
        }
    }
}




