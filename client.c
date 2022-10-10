#include <stdio.h>
#include "ModbusAp.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define address "172.18.16.1"
#define PORT 502

int main(){
    uint16_t val[10];

    transactionIdentifier = 0;

    Read_h_regs(address, PORT, 2, 5, val);

    uint16_t resultado[4];
    for(int i = 0 ; i < 4 ; i++){
        resultado[i] = val[i] + val[i + 1];  //Somar valores do registo atual e seguinte
    }

    Write_multiple_regs(address, PORT, 2, 4, resultado);
}