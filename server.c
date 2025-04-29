#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
typedef struct {
    SOCKET socket;
    char name[BUFFER_SIZE];
} ClientInfo;

ClientInfo clients[MAX_CLIENTS];



int clientCount = 0;
CRITICAL_SECTION cs;

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

DWORD WINAPI handleClient(LPVOID clientInfoPtr) {
    ClientInfo* client = (ClientInfo*)clientInfoPtr;
    SOCKET clientSocket = client->socket;
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesReceived] = '\0';  

        EnterCriticalSection(&cs);
        for (int i = 0; i < clientCount; i++) {
            if (clients[i].socket != clientSocket) {
                send(clients[i].socket, buffer, strlen(buffer), 0);
            }
        }
        LeaveCriticalSection(&cs);
    }
    EnterCriticalSection(&cs);
    for (int i = 0; i < clientCount; i++) {
        if (clients[i].socket == clientSocket) {
            for (int j = i; j < clientCount - 1; j++) {
                clients[j] = clients[j + 1];
            }
            clientCount--;
            break;
        }
    }
    LeaveCriticalSection(&cs);
    printf("Client disconnected.\n");
    closesocket(clientSocket);
    return 0;
}


int main(){
    initializeSock();
    SOCKET serverSocket = createServerSocket();
    bindSocket(serverSocket);
    listenClients(serverSocket);
    InitializeCriticalSection(&cs);
    struct sockaddr_in clAddr;
    int addrLen = sizeof(clAddr);

    while(1){
        SOCKET clientSocket = accept(serverSocket,(struct sockaddr*)&clAddr,&addrLen);
        EnterCriticalSection(&cs);
        if(clientCount < MAX_CLIENTS){
            clients[clientCount++].socket = clientSocket;
            ClientInfo *pClient = &clients[clientCount - 1];
            CreateThread(NULL,0,handleClient,pClient,0,NULL);
        }else{
            char *msg = "Server full!.. try later\n";
            send(clientSocket,msg,strlen(msg),0);
            closesocket(clientSocket);
        }
        LeaveCriticalSection(&cs);
    }
    DeleteCriticalSection(&cs);
    closesocket(serverSocket);
    WSACleanup();
    
}