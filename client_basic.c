#include <stdio.h>
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 100
#define PORT 5502
#define address INADDR_ANY

void comsStart(int sockfd){
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

int main(){
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = address;

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == -1){
        printf("\nErro na criação do socket\n\n");
        return 1;
    }
    else
        printf("\nSocket Criado com Sucesso\n");
    
    if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) == -1){
        printf("\nErro na ligação ao servidor. A sair...\n\n");
        return 1;
    }
    else
        printf("\nLigado com sucesso ao servidor\n");

    comsStart(sockfd);
    shutdown(sockfd, 2);
}