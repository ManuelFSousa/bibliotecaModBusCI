#include <stdio.h>
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void comsStart(char *pacote, int sockfd){
    while(1){
        char request[MAX], received[MAX];

        printf("\nEnsira o seu pedido: "); scanf("%s", request);
        request[strlen(request)] = '\0';
        
        if(send(sockfd, request, strlen(request), 0) == -1){
            printf("\nErro na Transmissão da Mensagem. A sair...");
            shutdown(sockfd, 2);
            return ;
        }

        if(recv(sockfd, received, MAX, 0) == -1){
            printf("\nErro na Recessão da Mensagem. A sair...");
            shutdown(sockfd, 2);
            return ;
        }
        received[strlen(received)] = '\0';
        printf("Mensagem Recebida: %s\n", received);
    }
}

Send_Modbus_request(struct sockaddr_in *server_add, int port, APDU, APDUlen, APDU_R){
    char pacote[100];

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == -1){
        printf("\nErro na criação do socket\n\n");
        return 1;
    }
    else
        printf("\nSocket Criado com Sucesso\n");
    return -1;

    if(connect(sockfd, (struct sockaddr*) &server_add, sizeof(server_add)) == -1){
        printf("\nErro na ligação ao servidor. A sair...\n\n");
        return 1;
    }
    else
        printf("\nLigado com sucesso ao servidor\n");

    comsStart(pacote, sockfd);

    shutdown(sockfd, 2);
}

Receive_Modbus_request(){

}

Send_Modbus_response(){

}