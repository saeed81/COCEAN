#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */

void error(const char *msg) { perror(msg); exit(0); }

char response[1024];

int main(int argc,char *argv[])
{
  /* first what are we going to send and where are we going to send it? */

  //http://oceandata.smhi.se/ssh/"+station+"/OBSERVATION",params = param
  int portno =        80;
  char *host =        "oceandata.smhi.se";
  char *message_fmt = "GET /ssh/viken/OBSERVATION?from=2018010100&too=2018013000 HTTP/1.1\r\nHost: oceandata.smhi.se\r\n\r\n";

  struct hostent *server;
  struct sockaddr_in serv_addr;
  int sockfd, bytes, sent, received, total;
  char message[1024];
  
  /* fill in the parameters */
  //sprintf(message,message_fmt,argv[1],argv[2]);
  printf("Request:\n%s\n",message_fmt);

  for (int i=0; i < 1023; i++){
    message[i] = *message_fmt;
    if (*message_fmt != '\0') message_fmt++;
  }
  
  message[1023] = '\0';
  printf("%s\n",message);

  /* create the socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");

  /* lookup the ip address */
  server = gethostbyname(host);
  if (server == NULL)error("ERROR, no such host server NULL");
  
  printf("we are here\n");
  
  /* fill in the structure */
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

  /* connect the socket */
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  /* send the request */
  total = strlen(message);
  
  printf("strln messge %d\n ", total);

  send(sockfd,message,strlen(message),0);

  //  Once you get that part working, your recv() logic is not parsing the server's response at all, which I assume is you simply not having gotten that far yet. But more importantly, the data being received is not null-terminated, but your call to printf() after recv() assumes that it is. You need to fix that as well, either like this:

  //int byte_count = recv(sockfd,response,sizeof(response)-1,0); // <-- -1 to leave room for a null terminator
  //response[byte_count] = 0; // <-- add the null terminator
  //printf("recv()'d %d bytes of data in buf\n",byte_count);
  
  /* close the socket */

  memset(response, 0, sizeof(response));
  total = sizeof(response)-1;
  received = 0;
  do {
    printf("RESPONSE: %s\n", response);
    // HANDLE RESPONSE CHUCK HERE BY, FOR EXAMPLE, SAVING TO A FILE.
    memset(response, 0, sizeof(response));
    bytes = recv(sockfd, response, 1023, 0);
    if (bytes < 0)
      printf("ERROR reading response from socket");
    if (bytes == 0)
      break;
    received+=bytes;
  } while (1); 


  close(sockfd);

  /* process response */
  //printf("Response:\n%s\n",response);

  return 0;
}













