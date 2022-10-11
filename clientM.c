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
    uint16_t val[10]; int ret, A, B, C, D;

    transactionIdentifier = 0;

    ret = Read_h_regs(address, PORT, 153, 1, val);
    A = val[0];
    if(ret != 0)
        return ret;
    
    printf("\n%d\n", (int)val[0]);

    ret = Read_h_regs(address, PORT, 154, 1, val);
    B = val[0];
    if(ret != 0)
        return ret;

    printf("\n%d\n", (int)val[0]);

    ret = Read_h_regs(address, PORT, 155, 1, val);
    C = val[0];
    if(ret != 0)
        return ret;

    printf("\n%d\n", (int)val[0]);
    
    val[0] = (uint16_t)55;
    ret =  Write_multiple_regs(address, PORT, 150, 1, val);
    if(ret != 0)
        return ret;

    if(C == 0)
        D = 5555;
    else if(C != 0)
        D = A + B; 

    val[0] = D;
    ret =  Write_multiple_regs(address, PORT, 151, 1, val);
    if(ret != 0)
        return ret;

    val[0] = D;
    ret =  Write_multiple_regs("10.227.113.1", PORT, 152, 1, val);
    if(ret != 0)
        return ret;
}