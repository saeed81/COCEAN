#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>      /* printf, sprintf */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include"strcomp.h"      
#define N (1024)

int main(int argc,char *argv[])
{
  //http://oceandata.smhi.se/ssh/"+station+"/OBSERVATION",params = param
  char response[N]  = {0};
  int portno        = 80;
  char *host        = "oceandata.smhi.se";
  //char message[]   = "GET /ssh/viken/OBSERVATION?from=2017100100&too=2018032000 HTTP/1.1\r\nHost: oceandata.smhi.se\r\nConnection:close\r\n\r\n";
  char message[]   = "GET /ssh/viken/OBSERVATION?from=2017100100&too=2018032000 HTTP/1.0\r\nHost: oceandata.smhi.se\r\n\r\n";
  int M = sizeof(message);
  //printf("%s\n",message);
  //printf("%d\n",M);

  struct hostent *server;
  struct sockaddr_in serv_addr;
  int sockfd, bytes;//, received;
  //printf("Request:\n%s\n",message);

  //for (int i=0; i < (N-1); i++){
  //   message[i] = *message_fmt;
  //  if (*message_fmt != '\0') message_fmt++;
  //}
  /* create the socket */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    printf("ERROR opening socket\n");
    return 1;
  }
  /* lookup the ip address */
  server = gethostbyname(host);
  if (server == NULL){
    printf("ERROR, no such host server NULL\n");
    return 1;
  }
    /* fill in the structure */
  //memset(&serv_addr,0,sizeof(serv_addr));

  char *cs =  (char *)&serv_addr;
  for (int i=0; i < sizeof(serv_addr); ++i) {
    *(cs+i) = 0;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  //memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);
  char *cb =  (char *)server->h_addr;
  char *ca =  (char *)&serv_addr.sin_addr.s_addr;
  for (int i=0; i<server->h_length; ++i){
    *(ca +i) = *(cb + i);
  }
  
  /* connect the socket */
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    printf("ERROR connecting");
    return 1;
  }

  /* send the request */
  send(sockfd,message,M,0);

  //  Once you get that part working, your recv() logic is not parsing the server's response at all, which I assume is you simply not having gotten that far yet. But more importantly, the data being received is not null-terminated, but your call to printf() after recv() assumes that it is. You need to fix that as well, either like this:

  //int byte_count = recv(sockfd,response,sizeof(response)-1,0); // <-- -1 to leave room for a null terminator
  //response[byte_count] = 0; // <-- add the null terminator
  //printf("recv()'d %d bytes of data in buf\n",byte_count);
  
  /* close the socket */

  //memset(response, 0, sizeof(response));
  //total = sizeof(response)-1;

  FILE *fl = fopen("output.dat","w+");

  //received = 0;
  //bytes = 0;
  do {
    // HANDLE RESPONSE CHUCK HERE BY, FOR EXAMPLE, SAVING TO A FILE.
    //memset(response, 0, sizeof(response));
    for (int i=0; i < N; ++i) {
      response[i] = 0;
    }
    bytes = recv(sockfd, response, N-1, 0);
    response[N-1] = 0;
    if (bytes > 0 ){
    //   response[bytes] = '\0';
      //printf("%s", response);

      fprintf(fl,"%s",response);
    }
    if (bytes < 0 )return 1;
    if (bytes == 0)break;
    //received+=bytes;
    //printf("here \n");
  } while(1); 

  //fclose(fl);
  
  //fl = fopen("output.dat","r");

  fseek(fl,0,SEEK_END);

  long int numc = ftell(fl);

  fseek(fl,0,SEEK_SET);

  char *content = (char *)malloc(numc * sizeof(char)); 

  char cf = '0';

  long int il = 0L;
  while((cf=fgetc(fl)) != EOF){
    *(content+il) = cf;
    il += 1;
  }
  
  printf("%s",content);
  //close(sockfd);
  printf("\n");
  
  /* process response */
  //printf("Response:\n%s\n",response);

  fclose(fl);

  return 0;
}













