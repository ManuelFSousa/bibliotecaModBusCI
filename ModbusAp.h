#ifndef _MODBUSAP_H
#define _MODBUSAP_H

#include <stdio.h>
#include "ModbusTCP.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//-------------------------------------------------------------------
//                      Código Funções Modbus
//-------------------------------------------------------------------
#define functionCode_h_regs             0x03
#define functionCode_w_multRegs         0x10

#define APDU_MAX_LENGTH 253

/**
 * @brief Function responsible to build the APDU byte array that contains the request to be sent to the server in order to read multiple holding registers
 * 
 * @param address Internet address of the server;
 * @param port Port through which the Modbus server can accessed;
 * @param st_r Initial modbus offset to be read;
 * @param n_r Number of modbus addresses to be read (including the initial one);
 * @param val Byte array where the read values will be stores.
 * @return Return 0 if successfull, -1 in case of variable error and 1...4 if a modbus exception occurs, being the number the corresponding exception.
*/
int Read_h_regs(char *address , int port, uint16_t st_r , uint16_t n_r , uint16_t *val);

/**
 * @brief Function responsible to build the APDU byte array that contains the request to be sent to the server in order to write multiple holding registers
 * 
 * @param addressInternet address of the server;
 * @param port Port through which the Modbus server can accessed;
 * @param st_r Initial modbus offset to be written;
 * @param n_r Number of modbus addresses to be written (including the initial one);
 * @param val Byte array with the vallues to be written in the modbus server.
 * @return Return 0 if successfull, -1 in case of variable error and 1...4 if a modbus exception occurs, being the number the corresponding exception.
*/
int Write_multiple_regs(char *address , int port, uint16_t st_r , uint16_t n_r , uint16_t *val);

#endif