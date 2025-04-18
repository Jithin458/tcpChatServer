#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>


#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

void initializeSock(){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0){
        printf("WsaStartup failed with error %ld\n",WSAGetLastError());
        exit(1);
    }
}

int createClientSocket(){
    SOCKET clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if  (clientSocket == INVALID_SOCKET){
        printf("Socket failed with error %ld\n",WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    return clientSocket;
}
void connectServer(SOCKET clientSocket){
    struct sockaddr_in seraddr;
    memset(&seraddr, 0,sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    seraddr.sin_port = htons(SERVER_PORT);
  
    if(connect(clientSocket,(struct sockaddr*)&seraddr,sizeof(seraddr))==SOCKET_ERROR){
      printf("Bind failed with error %d\n",WSAGetLastError());
      closesocket(clientSocket);
      WSACleanup();
      exit(1);
    }
  
  }

  void chat(SOCKET clientSocket){
    char buffer[BUFFER_SIZE];
    while(1){
        printf("You: ");
        fgets(buffer, sizeof(buffer),stdin);
        send(clientSocket,buffer, strlen(buffer),0);

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer),0);
        if(bytesReceived > 0){
            buffer[bytesReceived] = '\0';
            printf("Server: %s\n",buffer);

        }else{
            printf("connection closed\n");
            break;
        }
    }
  }

  int main(){
  initializeSock();

  SOCKET clientSocket = createClientSocket();
  connectServer(clientSocket);
  chat(clientSocket);

  closesocket(clientSocket);
  WSACleanup();

  }