/*
 * sim_params.cc
 *
 *  Created on: 22 June 2024
 *      Author: MONDALS
 */
#include <string.h>
#include <math.h>
#include <omnetpp.h>
#include "sim_params.h"

using namespace std;
using namespace omnetpp;

double olt_onu_distance = 20;                                           // OLT-ONU distance (km)
double light_speed = 2e5;                                               // speed of light in fiber 2 x 10^5 km/s
double pon_link_datarate = 1e9;                                         // PON link datarate = 1 Gbps

int const pkt_sz_min = 64;                                              // Ethernet packet size - minimum (bytes)
int const pkt_sz_max = 1542;                                            // Ethernet packet size - maximum (bytes)
int pkt_sz_avg = ceil((pkt_sz_min + pkt_sz_max)/2);                     // Average packet size (bytes)
int grant_reqst_size = 64;                                              // Grant/Request packet size (bytes)

double onu_buffer_capacity = 10e6;                                      // ONU buffer capacity (bytes)
double T_guard = 5e-6;                                                  // guard time for each ONU
//double onu_max_grant = 38437.5;                                            // maximum grant size for ONUs (Bytes)

