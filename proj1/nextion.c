#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "nextion.h"

#define TRUE 1
#define FALSE 0

char NexGetPageID(void)

{

  char rx[20];

  char page = 0xFF;

  int x =0;

  int i;

  unsigned int count = 0;

  unsigned int check = 0;

  

  NexCommand("sendme");

  delay(3);

 

  while (check == 0)

  {

    x = serialDataAvail (fd);

    for(i=0; i< x; i++)

    {

      rx[i] = serialGetchar (fd);

    }

    if((255 == rx[x-1]) && (255 == rx[x-2]) && (255 == rx[x-3]))

    {

      check = 1;

      if(102 == rx[0])

      {

        printf("page (from fn) = %d\n", rx[1]);

        page = rx[1]; 

      }

      else

      {

        page = 0xFF; 

      }

    }

    count++;

    if (count >= 10)

    {

      check = 1;

      page = 0xFF; 

    }

    delay (3) ;  

  }

  return page;  

}



void NexCommand(char *command)

{

  char tail[3] = {0xFF, 0xFF, 0xFF};

  fflush (stdout) ;

  serialPuts (fd, command) ;

  serialPuts (fd, tail) ;

}



void NexGotoPage(int page_id)

{

  char command[7];

  

  strcpy(command, "page ");

  switch (page_id)

  {

    case 0: strcat(command, "0");

            break;

    case 1: strcat(command, "1");

            break;

    case 2: strcat(command, "2");

            break;  

    case 3: strcat(command, "3");

            break;  

    case 4: strcat(command, "4");

            break;  

    case 5: strcat(command, "5");

            break;          

  }

  

  printf( "goto page: %s\n", command);

  NexCommand(command);

  

}



void NexSendTxt(char *object, char *text)

{

  char command[100];

  strcpy(command, object);

  strcat(command, ".txt=");

  strcat(command, "\"");

  strcat(command, text);

  strcat(command, "\"");

  printf("Text command = %s\n",command);

  NexCommand(command);

}

/*

void NexSendNum(char *object, int num)

{

  char command[10];

  char number[10];

  strcpy(command, object);

  strcat(command, ".val=);

  //strcat(command, text);

  printf("Text command = %s\n",command);

  NexCommand(command);

}



void NexSetVisibility(char *object, char vis)

{

  char command[10];

  strcpy(command, "vis ");

  strcat(command, object);

  strcat(command, ',');

  if(TRUE == vis)

  {

    strcat(command, '1');  

  }

  else

  {

    strcat(command, '0');  

  }

  printf("Vis command = %s\n",command);

//  NexCommand(command);

}*/
