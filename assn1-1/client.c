#include<sys/socket.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define MAXBUF 1024

int main()
{
  struct sockaddr_in server_addr;
  int server_sockfd;
  int server_addr_len;
  char buf[MAXBUF];

  if(0> (server_sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    printf("Error creating socket\n");

   server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(8000);
  server_addr_len = sizeof(server_addr);

  connect(server_sockfd, (struct sockaddr *) &server_addr, 
      server_addr_len);
  while(1)
  {
    memset(buf, 0x00, MAXBUF);

    fgets(buf, MAXBUF, stdin);
    if(write(server_sockfd, buf, MAXBUF) <= 0)
    {
      return 0;
    }

    if (strcmp(buf, "Q\n") == 0)
    {
      close(server_sockfd);
      break;
    }

    if(read(server_sockfd, buf, MAXBUF) <= 0)
    {
      return 0;
    }

    fputs(buf, stdout);
  }

  return 0;
}
