#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

main()
{
   int sd, psd;
   struct   sockaddr_in name,client;
   char   buf[1024],*ipaddress;
   int    cc,clilen;
   clilen=sizeof(client);

   sd = socket (AF_INET,SOCK_STREAM,0);
   name.sin_family = AF_INET;
   name.sin_addr.s_addr = htonl(INADDR_ANY);
   name.sin_port = htons(12345);

   bind( sd, (struct sockaddr*) &name, sizeof(name) );
   listen(sd,1);
   psd = accept(sd, (struct sockaddr *)&client, &clilen);
   //ipaddress=inet_ntop(AF_INET,&client.sin_addr, ipaddress, clilen);
   //ipaddress=inet_ntoa(client.sin_addr);
   //printf("client address :%s\n",ipaddress);
   close(sd);

   for(;;) {
   	cc=recv(psd,buf,sizeof(buf),0) ;
        if (cc == 0) exit (0);
   	buf[cc] = '\0';
   	printf("message received: %s\n", buf);
   }
}

