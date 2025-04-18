#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

void initializeSock(){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0){
        printf("WsaStartup failed with error %ld\n",WSAGetLastError());
        exit(1);
    }
}

int createServerSocket(){
    SOCKET serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if  (serverSocket == INVALID_SOCKET){
        printf("Socket failed with error %ld\n",WSAGetLastError());
        WSACleanup();
        exit(1);
    }
    return serverSocket;
}
void bindSocket(SOCKET serverSocket){
  struct sockaddr_in seraddr;
  memset(&seraddr, 0,sizeof(seraddr));
  seraddr.sin_family = AF_INET;
  seraddr.sin_addr.s_addr = INADDR_ANY;
  seraddr.sin_port = htons(SERVER_PORT);

  if(bind(serverSocket,(struct sockaddr*)&seraddr,sizeof(seraddr))==SOCKET_ERROR){
    printf("Bind failed with error %d\n",WSAGetLastError());
    closesocket(serverSocket);
    WSACleanup();
    exit(1);
  }

}
void listenClients(SOCKET serverSocket){
    if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR){
        printf("Listen failed with error %dld\n",WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        exit(1);

    }
    printf("Server listening on port %d..\n",SERVER_PORT);
}

void handleClient(SOCKET clientServer){
    char buffer[BUFFER_SIZE];
    int bytesRecieved;

    while ((bytesRecieved = recv(clientServer,buffer,sizeof(buffer),0)) >0){
        buffer[bytesRecieved] = '\0';
        printf("Client:%s\n",buffer);
        send(clientServer,buffer,bytesRecieved,0);
    }
    if(bytesRecieved ==0){
        printf("client disconnected\n");
    }else{
        printf("recv failed with error %ld\n",WSAGetLastError());
    }
    closesocket(clientServer);
}

int main(){
    initializeSock();
    SOCKET serverSocket = createServerSocket();
    bindSocket(serverSocket);
    listenClients(serverSocket);

    while(1){
        struct sockaddr_in claddr;
        int claddr_size = sizeof(claddr);
        SOCKET clientServer = accept(serverSocket,(struct sockaddr*)&claddr,&claddr_size);

        if(clientServer == INVALID_SOCKET){
            printf("Accept failed with error %ld\n",WSAGetLastError());
        }else{
            printf("client connected\n");
            handleClient(clientServer);
        }
    }
    closesocket(serverSocket);
    WSACleanup();
    
}