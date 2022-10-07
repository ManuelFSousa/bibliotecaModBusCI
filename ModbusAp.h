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

int Read_h_regs(char *address , int port, uint16_t st_r , uint16_t n_r , uint16_t *val);

int Write_multiple_regs(char *address , int port, uint16_t st_r , uint16_t n_r , uint16_t *val);

int Get_Request();

int Send_Request();

#endif