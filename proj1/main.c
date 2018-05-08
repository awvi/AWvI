#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "main.h"
#include "nextion.h"
#include "dht.h"
#include <time.h>

#define MS_30 500

void Read_Serial(char *data);
unsigned int init(void);
void ShowTime(void);
void Ambient(void);
int fd;
char Page_ID;

int main ()
{
  unsigned int Task_30ms ;
  unsigned int ret;
  char data[30];
  Page_ID = 0xFF;
  ret = init();
  if (1 == ret)
  {
    return 1;
  }
  Task_30ms = millis();
  for(;;)
  {
    if(MS_30 <= (millis() - Task_30ms))
    {
      Task_30ms = millis();
      Read_Serial(data);
      if(NEX_CURR_PAGE_ID == data[0])
      {
        Page_ID = data[1];
      }
      switch (Page_ID)
      {
        case 0: ShowTime();
                printf("page 0\n");
                Page_ID = 0xFF;
                break;
        case 1:
                printf("page 1\n");
                Ambient();
                Page_ID = 0xFF;
                break;  
        case 2:
                printf("page 2\n");
                Page_ID = 0xFF;
                break;
        case 3:
                printf("page 3\n");  
                Page_ID = 0xFF;
                break;  
        case 4:
                printf("page 4\n");
                Page_ID = 0xFF;
                break;     
        default:
                break;   
      }                
    }
  }
} 

void Read_Serial(char *data)
{
  int RxCount =0;
  char rx[100];
  int count;

  RxCount = serialDataAvail (fd);
  for(count=0; count< RxCount; count++)
  {
    rx[count] = serialGetchar (fd);
  }

  if((255 == rx[RxCount-1]) && (255 == rx[RxCount-2]) && (255 == rx[RxCount-3]))
  {
    for(count=0; count< (RxCount-3); count++)
    {
      data[count] = rx[count];
      //printf("%d ",data[count]);
    }
    printf("\n");
  }
  else
  {
   // printf("No Data\n");
  }
}

unsigned int init(void)
{
  unsigned int returntype = 0;

  if ((fd = serialOpen ("/dev/ttyS2", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    returntype = 1 ;
  }

  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    returntype = 1 ;
  }
  return returntype;
}


void ShowTime(void)
{
  char temp[30]; 

  time_t current_time = time(NULL);
  struct tm *tm = localtime(&current_time);
  strftime(temp, sizeof(temp), "%I:%M", tm);
  NexSendTxt("clock", temp); 
  strftime(temp, sizeof(temp), "%S %p", tm);
  NexSendTxt("APM", temp);
  strftime(temp, sizeof(temp), "%a %d %b %Y", tm);
  NexSendTxt("calender", temp);
}

void Ambient (void)
{
  static char task_count = 5;
  if (task_count == 5)
  {
    printf("call DHT22\n");
    read_dht_data();
  }
  else if (task_count == 0)
  {
    task_count = 6;
  }
  task_count--;
}
