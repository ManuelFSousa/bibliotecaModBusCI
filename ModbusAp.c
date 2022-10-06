#include <stdio.h>
#include "ModbusAp.h"
#include "ModbusTCP.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int Read_h_regs(struct sockaddr_in *server_add , int port, uint32_t st_r , uint16_t n_r , uint16_t *val){

    if(server_add == NULL || port < 0 || port > 65535 || st_r < 1 || st_r > 65536 || n_r > 65536-1+1 || st_r + n_r - 1 > 65536){
        printf("\nParameter Error on function Read_h_regs");
        return -1;
    }

    int APDU_length = 5;

    uint8_t APDU[APDU_length], resposta[APDU_MAX_LENGTH], respostaLength;
    int count, count2;

    APDU[0] = (uint8_t) functionCode_h_regs;
    APDU[1] = (uint8_t) (st_r >> 8); 
    APDU[2] = (uint8_t) (st_r & 0xff);
    APDU[3] = (uint8_t) (n_r >> 8);      // shift the higher 8 bits
    APDU[4] = (uint8_t) (n_r & 0xff);    // mask the lower 8 bits

    printf("\nSent Command: ");
    for(count = 0 ; count < APDU_length ; count++)
        printf("%u ", APDU[count]);
    printf("\n");

    if(Send_Modbus_request(server_add, port, APDU, APDU_length, resposta, &respostaLength) < 0){
        printf("\nError while sending the request (Read Holding Registers)\n\n");
        return -1;
    }

    printf("\nReceived Message: ");
    for(count = 0 ; count < APDU_MAX_LENGTH ; count++)
        printf("%u", resposta[count]);
    printf("\n");

    if(resposta[0] = functionCode_h_regs + 0x80){
        if(resposta[1] == 0x01){
            printf("\nException Code 01: Function Code Unsupported\n\n");
            return -1;
        }
        else if(resposta[1] == 0x02){
            printf("\nException Code 02: Registers Read Address Error\n\n");
            return -2;
        }
        else if(resposta[1] == 0x03){
            printf("\nException Code 03: Quantity of Registers out of boundaries\n\n");
            return -3;
        }
        else if(resposta[1] == 0x04){
            printf("\nException Code 04: Error Reading the Registers\n\n");
            return -4;
        }
    }

    count2 = 2;
    for(count = 0 ; count < n_r; count++){
        val[count] = (resposta[count2] >> 8) + resposta[count2 + 1];
        count2 += 2;
    }

    return n_r;
}

int Write_multiple_regs(struct sockaddr_in *server_add , int port, uint32_t st_r , uint16_t n_r , uint16_t *val){
    if(server_add == NULL || port < 0 || port > 65535 || st_r < 1 || st_r > 65536 || n_r > 65536-1+1 || st_r + n_r - 1 > 65536 || val == NULL){
        printf("\nParameter Error on function Read_h_regs");
        return -1;
    }

    uint8_t APDU[APDU_MAX_LENGTH], resposta[APDU_MAX_LENGTH], respostaLength;
    int count, count2;

    APDU[0] = (uint8_t) functionCode_w_multRegs;
    APDU[1] = (uint8_t) (st_r >> 8); 
    APDU[2] = (uint8_t) (st_r & 0xff);
    APDU[3] = (uint8_t) (n_r >> 8);      // shift the higher 8 bits
    APDU[4] = (uint8_t) (n_r & 0xff);    // mask the lower 8 bits
    APDU[5] = (uint8_t) (n_r*2);

    count2 = 6;
    for(count = 0 ; count < n_r ; count++){
        APDU[count2] = (uint8_t) (val[count] >> 8);
        APDU[count2 + 1] = (uint8_t) (val[count] & 0xff);
        count2 += 2;
    }
    

    printf("\nSent Command: ");
    for(count = 0 ; count < APDU_MAX_LENGTH ; count++)
        printf("%u", APDU[count]);
    printf("\n");

    if(Send_Modbus_request(server_add, port, APDU, 5, resposta, &respostaLength) < 0){
        printf("\nError while sending the request (Write Multiple Registers)\n\n");
        return -1;
    }

    printf("\nReceived Message: ");
    for(count = 0 ; count < APDU_MAX_LENGTH ; count++)
        printf("%u ", resposta[count]);
    printf("\n");

    if(resposta[0] = 0x90){
        if(resposta[1] == 0x01){
            printf("\nException Code 01: Function Code Unsupported\n\n");
            return -1;
        }
        else if(resposta[1] == 0x02){
            printf("\nException Code 02: Registers Write Address Error\n\n");
            return -2;
        }
        else if(resposta[1] == 0x03){
            printf("\nException Code 03: Quantity of Registers out of boundaries\n\n");
            return -3;
        }
        else if(resposta[1] == 0x04){
            printf("\nException Code 04: Error Writing the Registers\n\n");
            return -4;
        }
    }

    int regWrite = (resposta[3] << 8) + (resposta[4]);

    return regWrite;
}

int Get_Request(){
    return -1;
}

int Send_Request(){
    return -1;
}