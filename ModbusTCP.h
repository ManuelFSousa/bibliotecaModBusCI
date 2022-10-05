#include <stdio.h>
#include <string.h>	
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/**
 * @brief Construct a new Send_Modbus_request object
 * 
 * @param server_add 
 * @param port 
 * @param APDU 
 * @param APDUlen 
 * @param APDU_R 
 */
Send_Modbus_request(struct sockaddr_in *server_add, int port, uint8_t* APDU, int APDUlen, uint8_t *APDU_R);

Receive_Modbus_request();

Send_Modbus_response();