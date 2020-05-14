/****************************************************************************
 * Program:
 *    Lab RPSPserver, RPSP Rock/Paper/Scissors Protocol - Server Code
 *    Brother Jones, CS 460
 * Author:
 *    Steven Mitch Patterson
 * Summary:
 *    A simple server in the internet domain using TCP and the port
 *    number is passed as an argument.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define BSIZE 255
#define ROCK 0
#define PAPER 1
#define SCISSORS 2
#define QUIT 3
#define BAD_MOVE -1

void error(const char *msg) {
   perror(msg);
   exit(1);
}
/**********************************************************************
 * Main
 ***********************************************************************/
int main(int argc,  char *argv[]) {
   
   int listner,  p1fd,  p2fd,  portno;
   socklen_t clilen;
   char buffer1[BSIZE];
   char buffer2[BSIZE];
   struct sockaddr_in serv_addr,  cli_addr;
   int n1, n2;

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(1);
   }
   
   listner = socket(AF_INET,  SOCK_STREAM,  0);
   
   if (listner < 0)
      error("ERROR opening socket\n");
   
   bzero((char *) &serv_addr,  sizeof(serv_addr));
   
   portno = atoi(argv[1]);
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   if (bind(listner,  (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      error("ERROR on binding\n");
   
   listen(listner, 5);
   clilen = sizeof(cli_addr);
   
   printf("SERVER STARTED!\n\n");
   
   while(1) {
      printf("\n\n======= STARTING A NEW GAME ========\n");
      
      //bool shouldContinue = true;
      int status;
      const char* welbuff = "==========Welcome to Rock Paper Scissors Game!!==========\n";
      
      // Player 1
      printf("Waiting for a player 1 to connect...\n");
      p1fd = accept(listner, (struct sockaddr *) &cli_addr, &clilen);
      if (p1fd < 0)
         error("Error Trying to accept player 1!\n");
      
      status = send(p1fd,welbuff,strlen(welbuff),0);
      
      // Player 2
      printf("Waiting for a player 2 to connect...\n");
      p2fd = accept(listner, (struct sockaddr *) &cli_addr, &clilen);
      if (p2fd < 0)
         error("Error Trying to accept player 2!\n");
      status = send(p2fd,welbuff,strlen(welbuff),0);
      
      int round = 0;
      
      /*************************************
       * MAIN LOOP
       *************************************/
      while(true) {
         printf( "\n------------- ROUND %d ------------\n", ++round);
         
         bzero(buffer1, BSIZE);
         bzero(buffer2, BSIZE);
         
         printf("Waiting for Player 1's Move...\n");
         n1 = recv(p1fd, buffer1, BSIZE, 0);
         if (n1 < 0) error("ERROR reading from socket (p1)\n");
         
         printf("Waiting for Player 2's Move...\n");
         n2 = recv(p2fd, buffer2, BSIZE, 0);
         if (n2 < 0) error("ERROR reading from socket (p2)\n");
         
         
         printf("(1)-%c v.s %c-(2)\n", buffer1[0], buffer2[0]);
         int n1,n2;
         char buff1[2];
         char buff2[2];
         
         buff1[0] = buffer1[0];
         buff1[1] = '\0';
         buff2[0] = buffer2[0];
         buff2[1] = '\0';
         //char buff2[2]; = buffer2[0];
         
         n1 = send(p1fd,buff2, 1, 0);
         n2 = send(p2fd,buff1, 1, 0);
         
      }
      /************
       * Close
       ************/
      close(p1fd);
      close(p2fd);
   }
   
   close(listner);
   
   return 0;
}
