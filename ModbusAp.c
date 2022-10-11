#include <stdio.h>
#include "ModbusAp.h"
#include "ModbusTCP.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int Read_h_regs(char *address , int port, uint16_t st_r , uint16_t n_r , uint16_t *val){

    if(address == NULL || port < 0 || port > 65535 || (int)st_r < 1 || (int)st_r > 65536 || (int)n_r > 65536-1+1 || (int)st_r + (int)n_r - 1 > 65536){
        printf("\nParameter Error on function Read_h_regs");
        return -1;
    }

    printf("\n************Reading Hold Registers************\n");

    int APDU_length = 5;

    uint8_t APDU[APDU_length], response[APDU_MAX_LENGTH];
    uint16_t responseLength;
    int count, count2;

    APDU[0] = (uint8_t) functionCode_h_regs;        //Function Code
    APDU[1] = (uint8_t) (st_r >> 8);                //Starting Address: MSB
    APDU[2] = (uint8_t) (st_r & 0xff);              //Starting Address: LSB
    APDU[3] = (uint8_t) (n_r >> 8);                 //Quantity of Register: MSB
    APDU[4] = (uint8_t) (n_r & 0xff);               //Quantity of Register: LSB

    printf("\nAPDU Built: ");
    for(count = 0 ; count < APDU_length ; count++)
        printf("%u ", APDU[count]);
    printf("\n");

    if(Send_Modbus_request(address, port, APDU, APDU_length, response, &responseLength) < 0){
        printf("\nError while sending the request (Read Holding Registers)\n\n");
        return -1;
    }

    printf("\nReceived Message: ");
    for(count = 0 ; count < responseLength ; count++)
        printf("%u ", response[count]);
    printf("\n");

    if(response[0] == functionCode_h_regs + 0x80){
        if(response[1] == 0x01){
            printf("\nException Code 01: Function Code Unsupported\n\n");
            return 1;
        }
        else if(response[1] == 0x02){
            printf("\nException Code 02: Registers Read Address Error\n\n");
            return 2;
        }
        else if(response[1] == 0x03){
            printf("\nException Code 03: Quantity of Registers out of boundaries\n\n");
            return 3;
        }
        else if(response[1] == 0x04){
            printf("\nException Code 04: Error Reading the Registers\n\n");
            return 4;
        }
    }

    count2 = 2;
    for(count = 0 ; count < n_r; count++){
        val[count] = (response[count2] >> 8) + response[count2 + 1];
        count2 += 2;
    }

    //return n_r;
    return 0;
}

int Write_multiple_regs(char *address , int port, uint16_t st_r , uint16_t n_r , uint16_t *val){
    if(address == NULL || port < 0 || port > 65535 || st_r < 1 || (int)st_r > 65536 || (int)n_r > 65536-1+1 || (int)st_r + (int)n_r - 1 > 65536 || val == NULL){
        printf("\nParameter Error on function Read_h_regs");
        return -1;
    }

    printf("\n************Writing Multiple Registers************\n");

    uint8_t APDU[APDU_MAX_LENGTH], response[APDU_MAX_LENGTH];
    uint16_t responseLength;
    int count, count2;

    APDU[0] = (uint8_t) functionCode_w_multRegs;            //Function Code
    APDU[1] = (uint8_t) (st_r >> 8);                        //Starting Address: MSB
    APDU[2] = (uint8_t) (st_r & 0xff);                      //Starting Address: LSB
    APDU[3] = (uint8_t) (n_r >> 8);                         //Quantity of Registers: MSB
    APDU[4] = (uint8_t) (n_r & 0xff);                       //Quantity of Registers: LSB 
    APDU[5] = (uint8_t) (n_r*2);                            //Byte Count 

    count2 = 6;
    for(count = 0 ; count < n_r ; count++){
        APDU[count2] = (uint8_t) (val[count] >> 8);         //Register Value: MSB
        APDU[count2 + 1] = (uint8_t) (val[count] & 0xff);   //Register Value: LSB
        count2 += 2;
    }
    
    uint16_t APDUlen = (6 + n_r*2);

    printf("\nAPDU Built: ");
    for(count = 0 ; count < APDUlen ; count++)
        printf("%u ", APDU[count]);
    printf("\n");

    if(Send_Modbus_request(address, port, APDU, APDUlen, response, &responseLength) < 0){
        printf("\nError while sending the request (Write Multiple Registers)\n\n");
        return -1;
    }

    printf("\nReceived Message: ");
    for(count = 0 ; count < responseLength ; count++)
        printf("%u ", response[count]);
    printf("\n");

    if(response[0] == 0x90){
        if(response[1] == 0x01){
            printf("\nException Code 01: Function Code Unsupported\n\n");
            return 1;
        }
        else if(response[1] == 0x02){
            printf("\nException Code 02: Registers Write Address Error\n\n");
            return 2;
        }
        else if(response[1] == 0x03){
            printf("\nException Code 03: Quantity of Registers out of boundaries\n\n");
            return 3;
        }
        else if(response[1] == 0x04){
            printf("\nException Code 04: Error Writing the Registers\n\n");
            return 4;
        }
    }

    int regWrite = (response[3] << 8) + (response[4]);

    //return regWrite;
    return 0;
}