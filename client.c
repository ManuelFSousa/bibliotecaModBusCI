#include <stdio.h>
#include "ModbusAp.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define address "127.0.0.1"
#define PORT 502

int main(){
    uint16_t val[10];
    
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(address);

    Read_h_regs(&servaddr, PORT, 2, 2, val);

    uint16_t resultado = val[0] + val[1];

    Write_multiple_regs(&servaddr, PORT, 2, 1, &resultado);
}