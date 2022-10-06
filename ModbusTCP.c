#include <stdio.h>
#include "ModbusTCP.h"
#include "ModbusAp.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int comsStart(int sockfd, uint8_t *PDU, int PDU_SIZE, uint8_t *APDU_R, int *APDU_r_length){
    int receivedSize, missingSize;

    if(send(sockfd, PDU, PDU_SIZE, 0) == -1){
        printf("\nPDU Transmission Error\n\n");
        return -1;
    }

    if(recv(sockfd, APDU_R, MBAP_SIZE, 0) == -1){
        printf("\nPDU Reception Error\n\n");
        return -1;
    }

    if((APDU_R[0] << 8 + APDU_R[1]) != transactionIdentifier){
        printf("\nUnmatched transaction identifier\n\n");
        return -1;
    }
    else if((APDU_R[2] << 8 + APDU_R[3]) != 0){
        printf("\nUnmatched Protocol Identifier\n\n");
        return -1;
    }

    APDU_r_length = (APDU_R[4] << 8 + APDU_R[5]) - 1;

    receivedSize = recv(sockfd, APDU_R, APDU_r_length, 0);
    if(receivedSize == -1){
        printf("\nAPDU Reception Error\n\n");
        return -1;
    }
    else if(receivedSize >= 0 && receivedSize < APDU_r_length){                                      //Not fully received, try a 2nd time to fetch the data
        missingSize = APDU_r_length - receivedSize;
        receivedSize = recv(sockfd, &APDU_R[(int)APDU_r_length - missingSize], missingSize, 0);      //(int)APDU_r_length - missingSize é o indíce onde ficou o primeiro espaçoç livre do vetor por não ter sido completamente recebido
        if(receivedSize != missingSize){
            printf("\nIt was not possible to fetch the complete request\n\n");
            return -1;
        }
    }
    return 0;
}

int Send_Modbus_request(struct sockaddr_in * server_add, int port, uint8_t * APDU, uint16_t APDUlen, uint8_t * APDU_R, uint16_t * APDU_Rlen){

    if(server_add == NULL || port < 0 || port > 65535 || APDU == NULL || APDUlen > APDU_MAX_LENGTH || APDU_R == NULL || APDU_Rlen == NULL){
        printf("\nParameter Error on function Read_h_regs");
        return -1;
    }

    uint8_t PDU[MBAP_SIZE + APDUlen];
    int count, PDU_SIZE, respostaLength;

    PDU[0] = (uint8_t) (transactionIdentifier >> 8);
    PDU[1] = (uint8_t) (transactionIdentifier & 0x00ff);
    PDU[2] = (uint8_t) (0 >> 8);
    PDU[3] = (uint8_t) (0 & 0x00ff);
    PDU[4] = (uint8_t) ((1 + APDUlen) >> 8);
    PDU[5] = (uint8_t) ((1 + APDUlen) & 0x00ff);
    PDU[6] = (uint8_t) (1);

    for(count = 0; count < APDUlen ; count++)
        PDU[count + 7] = APDU[count];

    transactionIdentifier++;

    printf("\nSent Command: ");
    for(count = 0 ; count < (APDUlen + MBAP_SIZE) ; count++)
        printf("%u ", PDU[count]);
    printf("\n");

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == -1){
        printf("\nSocket Creation Error\n\n");
        return -1;
    }

    if(connect(sockfd, (struct sockaddr*) &server_add, sizeof(server_add)) == -1){
        close(sockfd);
        printf("\nServer Connection Error\n\n");
        return -1;
    }

    PDU_SIZE = MBAP_SIZE + APDUlen;
    if(comsStart(sockfd, PDU, PDU_SIZE, APDU_R, APDU_Rlen) == -1){
        close(sockfd);
        return -1;
    }

    printf("\nReceived Message: ");
    for(count = 0 ; count < APDU_Rlen ; count++)
        printf("%u ", APDU_R[count]);
    printf("\n");

    shutdown(sockfd, 2);
    return 0;
}

int Receive_Modbus_request(){

}

int Send_Modbus_response(){

}