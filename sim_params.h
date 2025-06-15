/*
 * sim_params.h
 *
 *  Created on: 22 June 2024
 *      Author: MONDALS
 */

#ifndef SIM_PARAMS_H_
#define SIM_PARAMS_H_

extern double olt_onu_distance;               // OLT-ONU distance (km)
extern double light_speed;                    // speed of light in fiber 2 x 10^5 km/s
extern double pon_link_datarate;              // PON link datarate = 1 Gbps

extern int const pkt_sz_min;                  // Ethernet packet size - minimum (bytes)
extern int const pkt_sz_max;                  // Ethernet packet size - maximum (bytes)
extern int pkt_sz_avg;                        // Average packet size (bytes)
extern int grant_reqst_size;                  // Grant/Request packet size (bytes)

extern double onu_buffer_capacity;            // ONU buffer capacity (bytes)
extern double T_guard;                        // guard time for each ONU
//extern double onu_max_grant;                  // maximum grant size for ONUs (Bytes)


#endif /* SIM_PARAMS_H_ */
