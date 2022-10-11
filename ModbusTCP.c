#include <stdio.h>
#include "ModbusTCP.h"
#include "ModbusAp.h"
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int comsStart(int sockfd, uint8_t *MBAPDU, int MBAPDU_SIZE, uint8_t *APDU_R, uint16_t* APDU_r_length){
    uint16_t receivedSize, missingSize;

    if(send(sockfd, MBAPDU, MBAPDU_SIZE, 0) == -1){
        printf("\nPDU Transmission Error\n\n");
        return -1;
    }

    if(recv(sockfd, APDU_R, MBAP_SIZE, 0) == -1){
        printf("\nPDU Reception Error\n\n");
        return -1;
    }
    
    if(((APDU_R[0] << 8) + APDU_R[1]) != transactionIdentifier){
        printf("\nUnmatched transaction identifier\n\n");
        return -1;
    }
    else if(((APDU_R[2] << 8) + APDU_R[3]) != 0){
        printf("\nUnmatched Protocol Identifier\n\n");
        return -1;
    }

    *APDU_r_length = ((APDU_R[4] << 8) + APDU_R[5]) - 1;

    receivedSize = recv(sockfd, APDU_R, *APDU_r_length, 0);
    if((int)receivedSize == -1){
        printf("\nAPDU Reception Error\n\n");
        return -1;
    }
    else if(receivedSize >= 0 && receivedSize < *APDU_r_length){                                      //Not fully received, try a 2nd time to fetch the data
        missingSize = *APDU_r_length - receivedSize;
        receivedSize = recv(sockfd, &APDU_R[(int)*APDU_r_length - missingSize], missingSize, 0);      //(int)APDU_r_length - missingSize é o indíce onde ficou o primeiro espaçoç livre do vetor por não ter sido completamente recebido
        if(receivedSize != missingSize){
            printf("\nIt was not possible to fetch the complete request\n\n");
            return -1;
        }
    }
    return 0;
}

int Send_Modbus_request(char *address, int port, uint8_t * APDU, uint16_t APDUlen, uint8_t * APDU_R, uint16_t *APDU_Rlen){

    if(address == NULL || port < 0 || port > 65535 || APDU == NULL || APDUlen > APDU_MAX_LENGTH || APDU_R == NULL || APDU_Rlen == NULL){
        printf("\nParameter Error on function Read_h_regs");
        return -1;
    }

    uint8_t MBAPDU[MBAP_SIZE + APDUlen];
    int count, MBAPDU_SIZE, respostaLength;

    MBAPDU[0] = (uint8_t) (transactionIdentifier >> 8);         //Transaction Identifier: MSB
    MBAPDU[1] = (uint8_t) (transactionIdentifier & 0x00ff);     //Transaction Identifier: LSB
    MBAPDU[2] = (uint8_t) (0 >> 8);                             //Protocol Identifier: MSB
    MBAPDU[3] = (uint8_t) (0 & 0xff);                           //Protocol Identifier: LSB
    MBAPDU[4] = (uint8_t) ((1 + APDUlen) >> 8);                 //Length: MSB
    MBAPDU[5] = (uint8_t) ((1 + APDUlen) & 0xff);               //Length: LSB
    MBAPDU[6] = (uint8_t) (2);                                  //Unit ID

    for(count = 0; count < APDUlen ; count++)
        MBAPDU[count + 7] = APDU[count];

    printf("\nPDU Built: ");
    for(count = 0 ; count < (APDUlen + MBAP_SIZE) ; count++)
        printf("%u ", MBAPDU[count]);
    printf("\n");

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_aton(address, &servaddr.sin_addr);

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == -1){
        printf("\nSocket Creation Error\n\n");
        return -1;
    }
    
    if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1){
        close(sockfd);
        printf("\nServer Connection Error\n\n");
        return -1;
    }

    MBAPDU_SIZE = MBAP_SIZE + APDUlen;
    if(comsStart(sockfd, MBAPDU, MBAPDU_SIZE, APDU_R, APDU_Rlen) == -1){
        close(sockfd);
        return -1;
    }

    transactionIdentifier++;

    printf("\nReceived Message: ");
    for(count = 0 ; count < *APDU_Rlen ; count++)
        printf("%u ", APDU_R[count]);
    printf("\n");

    shutdown(sockfd, 2);
    return 0;
}