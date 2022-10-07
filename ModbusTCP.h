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

int Send_Modbus_request(char *address, int port, uint8_t * APDU, uint16_t APDUlen, uint8_t * APDU_R, uint16_t * APDU_Rlen);

int Receive_Modbus_request();

int Send_Modbus_response();

#endif