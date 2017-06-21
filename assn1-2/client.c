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
  int f = 0; //1 = Send, -1 = Recv
  char buf[MAXBUF];

  if(0> (server_sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    printf("Error creating socket\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(8080);
  server_addr_len = sizeof(server_addr);

  connect(server_sockfd, (struct sockaddr *) &server_addr, 
      server_addr_len);

  while(1)
  {
    if(f != -1)
    {
      memset(buf, 0x00, MAXBUF);

      fgets(buf, MAXBUF, stdin);
      if(write(server_sockfd, buf, MAXBUF) <= 0)
      {
        return 0;
      }

      if (f == 0)
      {
        if (strcmp(buf, "SEND\n") == 0)
          f = 1;
        else if (strcmp(buf, "RECV\n") == 0)
          f = -1;
        else
          printf("select SEND or RECV\n");

        continue;
      }

      if (strcmp(buf, "Q\n") == 0)
      {
        close(server_sockfd);
        break;
      }
    }
    else
    {
    
      if(read(server_sockfd, buf, MAXBUF) <= 0)
      {
        return 0;
      }
      
      if (strcmp(buf, "LAST_MSG") == 0)
      {
        close(server_sockfd);
        break;
      }

      fputs(buf, stdout);
    }
  }

  return 0;
}
