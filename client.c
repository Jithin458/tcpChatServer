#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>


#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024


CRITICAL_SECTION cs;  



void initializeSock(){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2), &wsaData)!=0){
        printf("WsaStartup failed with error %ld\n",WSAGetLastError());
        exit(1);
    }
}

int createClientSocket(){
    SOCKET clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    char buffer[20];
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

DWORD WINAPI handleRecv(LPVOID socketDesc) {
    SOCKET clientSocket = *(SOCKET*)socketDesc;
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            buffer[bytesReceived] = '\0';
            EnterCriticalSection(&cs);
            printf("\n%s\n",buffer);
            printf("You:");
    
            LeaveCriticalSection(&cs);   
    }
    printf("Disconnected from server!!");
    closesocket(clientSocket);
    return 0;
}

DWORD WINAPI handleSend(LPVOID socketDesc) {
    SOCKET clientSocket = *(SOCKET*)socketDesc;
    char buffer[BUFFER_SIZE];
    char messageWithName[BUFFER_SIZE + 50];
    char clientName[BUFFER_SIZE];

    printf("Enter your name: ");
    fgets(clientName, sizeof(clientName), stdin);
    clientName[strcspn(clientName, "\n")] = 0;  

    while (1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; 
        snprintf(messageWithName, sizeof(messageWithName), "%s: %s", clientName, buffer);
        if (send(clientSocket, messageWithName, strlen(messageWithName), 0) == SOCKET_ERROR){
            printf("Failed to send message.\n");
            break;
        }
    }
    closesocket(clientSocket);  
    return 0;
    }

   


  int main(){
  initializeSock();
  SOCKET clientSocket = createClientSocket();
  connectServer(clientSocket);
  InitializeCriticalSection(&cs);
  CreateThread(NULL, 0, handleSend, &clientSocket, 0, NULL);
  CreateThread(NULL, 0, handleRecv, &clientSocket, 0, NULL);
 
  while (1) {
    Sleep(1000);
}
DeleteCriticalSection(&cs); 
closesocket(clientSocket);
WSACleanup();

  }