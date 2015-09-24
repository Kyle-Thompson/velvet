#include "common.h"

int main() {
  int listenfd = 0, connfd = 0;
  
  struct sockaddr_in serv_addr;
 
  char sendBuff[BUFFER+1];
  char recvBuff[BUFFER+1]; 
 
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;    
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(PORT);    
 
  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  if (listen(listenfd, 10) == -1) {
    printf("Failed to listen\n");
    return -1;
  }

  connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
  
  pid_t pid = fork();
  if (pid) {
    while (strncmp(recvBuff, "exit", 4) != 0) {
      if(recv(connfd, recvBuff, sizeof(recvBuff), 0) < 0)
        printf("Error: Receive\nErrno: %d\n", errno);
      recvBuff[BUFFER] = '\n';

      printf("Client: %s", recvBuff);
    }
    
    wait(&pid);
    close(connfd);
    
  } else {
    while (strncmp(sendBuff, "exit", 4) != 0) {
      fputs("(Server) Enter a message: ", stdout);
      fgets(sendBuff, sizeof(sendBuff), stdin);
      
      if(send(connfd, sendBuff, sizeof(sendBuff), 0) < 0)
        printf("Error: Send\nErrno: %d\n", errno);
    }
    close(connfd);
    exit(0);
  }

  close(listenfd); 
 
  return 0;    

}