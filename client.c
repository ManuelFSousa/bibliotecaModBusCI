#include <stdio.h>
#include "ModbusAp.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define address "127.0.0.1"
#define PORT 5502

int main(){
    uint16_t valor[2];

    valor[0]=23;
    valor[1]=51;

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = (uint) address;

    Read_h_regs(&servaddr, PORT, (uint16_t) 3, (uint16_t) 2, valor);
}