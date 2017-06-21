#include<sys/socket.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define MAXBUF 1024

int main(int argc, char **argv)
{
  struct sockaddr_in server_addr, client_addr;
  int server_sockfd, client_sockfd;
  int client_addr_len;
  int f = 0;
  int n = 0;
  int i = 0;
  char buf[MAXBUF];
  char queue[MAXBUF][MAXBUF];

  if (0 > (server_sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    printf("Error creating socket\n");
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_addr.sin_port = htons(8080);

  if (0 > (bind(server_sockfd, (struct sockaddr *)&server_addr,
          sizeof(server_addr))))
    printf("Error binding\n");

  if (0 > listen(server_sockfd, 5))
    printf("Error listening\n");

  client_addr_len = sizeof(client_addr);

  while(1)
  {
    if(0 > (client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_len)))
      printf("Error accepting\n");

    while(1)
    {
      memset(buf, 0x00, MAXBUF);

      if (f != -1)
      {
        if(read(client_sockfd, buf, MAXBUF) <= 0)
        {
          close(client_sockfd);
          break;
        }

        if (f == 0)
        {
          if(strcmp(buf, "SEND\n") == 0)
            f = 1;
          else if (strcmp(buf, "RECV\n") == 0)
            f = -1;

          continue;
        }

        if (strcmp(buf, "Q\n") == 0)
          break;

        strncpy(queue[n++], buf, MAXBUF);
      }
      else 
      {
        if (n > i)
          strncpy(buf, queue[i++], MAXBUF);
        else if(n == i)
        {
          sprintf(buf, "LAST_MSG");
          n = 0;
          i = 0;
          memset(queue, 0x00, MAXBUF * MAXBUF);
        }

        if(write(client_sockfd, buf, MAXBUF) <= 0)
        {
          close(client_sockfd);
          break;
        }
      }
    }
    f = 0;
    close(client_sockfd);
  }
  close(server_sockfd);
  return 0;
}
