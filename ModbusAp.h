#ifndef _MODBUSAP_H
#define _MODBUSAP_H

#include <stdio.h>
#include "modbustcp.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//-------------------------------------------------------------------
//                           Código Funções Modbus
//-------------------------------------------------------------------
#define functionCode_h_regs             0x03
#define functionCode_w_multRegs         0x10

#define MAXbytes 253

/**
 * @brief 
 * 
 * @param server_add 
 * @param port 
 * @param st_r 
 * @param n_r 
 * @param val 
 * @return int 
 */
int Read_h_regs(struct sockaddr_in *server_add , int port, uint32_t st_r , uint16_t n_r , uint16_t *val);

/**
 * @brief 
 * 
 * @param server_add 
 * @param port 
 * @param st_r 
 * @param n_r 
 * @param val 
 * @return int 
 */
int Write_multiple_regs(struct sockaddr_in *server_add , int port, uint32_t st_r , uint16_t n_r , uint16_t *val);

int Get_Request();

int Send_Request();

#endif