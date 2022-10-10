#ifndef _MODBUSTCP_H
#define _MODBUSTCP_H

#include <stdio.h>
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MBAP_SIZE 7

static uint16_t transactionIdentifier;

/**
 * @brief This function adds the MBAP header to the APDU and deals with the communication with the server through a socket, from the request to the response.
 * 
 * @param address Internet address of the server;
 * @param port Port through which the Modbus server can accessed;
 * @param APDU APDU byte structure;
 * @param APDUlen Size of the APDU byte structure;
 * @param APDU_R Pointer to the memory location where the response of the server will be stored;
 * @param APDU_Rlen Length of the server response byte structure.
 * @return Return -1 in case of an error, 0 if everything is ok.
*/
int Send_Modbus_request(char *address, int port, uint8_t * APDU, uint16_t APDUlen, uint8_t * APDU_R, uint16_t * APDU_Rlen);

#endif