/*******************************************************************************
* File Name: weat.c
* Author: Manmohan
* Version: 1.0
* Date: 31/05/2018
* Description: Phrase & displaay weather forecast from http://openweathermap.org
*******************************************************************************/

/*******************************************************************************
    INCLUDES
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "jsmn.h"
#include "weat.h"
#include "requests.h"
/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/
static weather_data weat_m[4];
static forecast_t forecast[4];
static char location[30];     
static int next_hr = 33;                           
/*******************************************************************************
    INTERNAL FUNCTION FORWARD DECLARATION
*******************************************************************************/
static void get_next_hr(int cur_hr);
static void display_weather(void);
static void update(int cur_dt);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s);
static void Clear_Forecast(void);
//static void readfile(char* filepath, char* fileContent);
static int conv_date(char *dt);
/*******************************************************************************
*  FUNCTIONS DEFINITION
*******************************************************************************/
/*******************************************************************************
* Function Name: weat_main()
* Description: Periodic function to be called by application.
*******************************************************************************/
void weat_main(void)
{
  int cur_hr;
  int cur_dt;
  int temp;
  int start;
  time_t current_time = time(NULL);
  struct tm *tm = gmtime(&current_time);
  cur_hr = tm->tm_hour;
  cur_dt = tm->tm_mday;
  temp = cur_hr/3;
  start = cur_hr%3;
  temp = temp*3;
  
  if(((0 == start)&&(temp == next_hr)) || (33 == next_hr))
  {
    get_next_hr(cur_hr);
    update(cur_dt);
  }
  display_weather();
}
/*******************************************************************************
* End of weat_main()
*******************************************************************************/

/*******************************************************************************
* Function Name: get_next_hr()
* Description: Calculates next update time slot.
*              (website updates weather every three hours UTC)
*******************************************************************************/ 
static void get_next_hr(int cur_hr)
{
  if (20 < cur_hr)
  {
    next_hr = 0;
  }
  else
  {
    next_hr = ((cur_hr/3)+1)*3;
  }
}
/*******************************************************************************
* End of get_next_hr()
*******************************************************************************/
 
/*******************************************************************************
* Function Name: display_weather()
* Description: Displays weather in nextion.
*******************************************************************************/ 
static void display_weather(void)
{
    int i;
    printf("%s\n_______________________\n",location);
    printf("TODAY\n____________\n");
    printf("temp = %s\n",weat_m[0].temperature);
    printf("temp_max  = %s\n",weat_m[0].temp_max);
    printf("temp_min = %s\n",weat_m[0].temp_min);
    printf("humidity = %s\n",weat_m[0].humidity);
    printf("Descr = %s\n",weat_m[0].Descr);
    printf("icon = %s\n",weat_m[0].icon);
    printf("date = %s\n",weat_m[0].date);
    printf("hr  = %s\n",weat_m[0].hr); 
    printf("____________\n");
    
    printf("+1\n____________\n");
    printf("temp_max = %s\n",weat_m[1].temp_max);
    printf("temp_min = %s\n",weat_m[1].temp_min);
    printf("icon = %s\n",weat_m[1].icon);
    printf("____________\n");
    
    printf("+2\n____________\n");
    printf("temp_max = %s\n",weat_m[2].temp_max);
    printf("temp_min = %s\n",weat_m[2].temp_min);
    printf("icon = %s\n",weat_m[2].icon);
    printf("____________\n");
    
    printf("+3\n____________\n");
    printf("temp_max = %s\n",weat_m[3].temp_max);
    printf("temp_min = %s\n",weat_m[3].temp_min);
    printf("icon = %s\n",weat_m[3].icon);
    printf("____________\n");
    
    printf("+4\n____________\n");
    printf("temp_max = %s\n",weat_m[4].temp_max);
    printf("temp_min = %s\n",weat_m[4].temp_min);
    printf("icon = %s\n",weat_m[4].icon);
    printf("____________\n");
    
    printf("TODAY FORECAST\n____________\n");
    for(i=0; i<4; i++)
    {
      printf("HR: %s\n",forecast[i].hr);
      printf("ICON: %s\n", forecast[i].icon);
    }
}
 
/*******************************************************************************
* End of display_weather()
*******************************************************************************/
 
/*******************************************************************************
* Function Name: update()
* Description: Updates weather from website and stores in global variable.
*******************************************************************************/ 
static void update(int cur_dt)
{
  int i;
  int r;
  jsmn_parser p;
  int forecast_cnt = 0;
  int temp_date;
  jsmntok_t t[MAX_TOKEN_COUNT]; /* We expect no more than 128 tokens */
  char JSON_STRING[BUFFER_SIZE];
  weather_data weather[MAX_WEAT_DATA];
  unsigned int temp_cnt=0;
  unsigned int temp_max_cnt=0;
  unsigned int temp_min_cnt=0;
  unsigned int humidity_cnt=0;
  unsigned int Descr_cnt=0;
  unsigned int icon_cnt=0;
  unsigned int dt_cnt=0;
  unsigned int hr_cnt=0;
  unsigned int time_start=0;
  unsigned int date_start=0;
  unsigned int str_length=0;
  req_t req;
  CURL *curl = requests_init(&req);

  requests_get(curl, &req, "http://api.openweathermap.org/data/2.5/forecast?q=thiruvananthapuram,IN&units=metric&cnt=25&appid=41d134a01debea1daff779fa7cba5c19"); /* submit GET request */
  //printf("Response Body:%s\n", req.text);
  requests_close(&req); /* always do this at the end, you'll have memory leaks if you don't */

  strcpy(JSON_STRING,req.text);
  //printf("Response Body:%s\n", JSON_STRING);

  /*todo*/ //replace with CULR call
  //readfile(JSON_FILE_PATH, JSON_STRING);
  /*todo*/
  
  jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return;
	}
 
	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) 
  {
    if (jsoneq(JSON_STRING, &t[i], "temp") == 0)
    {
    
        str_length = t[i+1].end - t[i+1].start;
        sprintf(weather[temp_cnt].temperature, "%.*s", str_length, JSON_STRING + t[i+1].start);
        temp_cnt++;
        i++;
    }
    else if (jsoneq(JSON_STRING, &t[i], "temp_min") == 0)
    {
        str_length = t[i+1].end - t[i+1].start;
        sprintf(weather[temp_min_cnt].temp_min, "%.*s", str_length, JSON_STRING + t[i+1].start);
        temp_min_cnt++;
        i++;
    }
    else if (jsoneq(JSON_STRING, &t[i], "temp_max") == 0)
    {
        str_length = t[i+1].end - t[i+1].start;
        sprintf(weather[temp_max_cnt].temp_max, "%.*s", str_length, JSON_STRING + t[i+1].start);
        temp_max_cnt++;
        i++;
    }
    else if (jsoneq(JSON_STRING, &t[i], "humidity") == 0)
    {
        str_length = t[i+1].end - t[i+1].start;
        sprintf(weather[humidity_cnt].humidity, "%.*s", str_length, JSON_STRING + t[i+1].start);
        humidity_cnt++;
        i++;
    }
    else if (jsoneq(JSON_STRING, &t[i], "description") == 0)
    {
        str_length = t[i+1].end - t[i+1].start;
        sprintf(weather[Descr_cnt].Descr, "%.*s", str_length, JSON_STRING + t[i+1].start);
        Descr_cnt++;
        i++;
    }
    else if (jsoneq(JSON_STRING, &t[i], "icon") == 0)
    {
        str_length = t[i+1].end-t[i+1].start;
        sprintf(weather[icon_cnt].icon, "%.*s", str_length, JSON_STRING + t[i+1].start);
        icon_cnt++;
      i++;
	  }
    else if (jsoneq(JSON_STRING, &t[i], "dt_txt") == 0)
    {
        str_length = t[i+1].end-t[i+1].start;
        time_start = t[i+1].start + 11;
        date_start = t[i+1].start + 8;
        sprintf(weather[dt_cnt].date, "%.*s", 2, JSON_STRING + date_start);
        sprintf(weather[hr_cnt].hr, "%.*s", 2, JSON_STRING + time_start);
        dt_cnt++;
        hr_cnt++;
        i++;
    }
    else if (jsoneq(JSON_STRING, &t[i], "name") == 0)
    {
        str_length = t[i+1].end-t[i+1].start;
        sprintf(location, "%.*s", str_length, JSON_STRING + t[i+1].start);
      i++;
	  }
	}
/* Update "weat_m" and "forecast" with data to be displayed */
  for (i=0; i<4; i++)
  {
    strcpy(weat_m[i].temperature, weather[i*8].temperature);
    strcpy(weat_m[i].temp_max, weather[i*8].temp_max);
    strcpy(weat_m[i].temp_min, weather[i*8].temp_min);
    strcpy(weat_m[i].humidity, weather[i*8].humidity);
    strcpy(weat_m[i].Descr, weather[i*8].Descr);
    strcpy(weat_m[i].icon, weather[i*8].icon);
    strcpy(weat_m[i].date, weather[i*8].date);
    strcpy(weat_m[i].hr, weather[i*8].hr);
  }
  Clear_Forecast();
  forecast_cnt = 0;
  for (i=1; ((i<MAX_WEAT_DATA) && (4 > forecast_cnt)); i++)
  {
    temp_date = conv_date(weather[i].date);
    if(cur_dt == temp_date)
    {
      strcpy(forecast[forecast_cnt].hr, weather[i].hr);
      strcpy(forecast[forecast_cnt].icon, weather[i].icon);
      //printf("%d\n",forecast_cnt);
      //printf("     %s-%s\n",forecast[forecast_cnt].hr,  forecast[forecast_cnt].icon);
      forecast_cnt++;
    }
  }

	
}
/*******************************************************************************
* End of display_weather()
*******************************************************************************/
 
/*******************************************************************************
* Function Name: jsoneq()
* Description: checks for JSON elements
*******************************************************************************/ 
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) 
{
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) 
  {
		return 0;
	}
	return -1;
}
/*******************************************************************************
* End of jsoneq()
*******************************************************************************/

/*******************************************************************************
* Function Name: Clear_Forecast()
* Description: clear forecast array
*******************************************************************************/ 
static void Clear_Forecast(void)
{
  int i;
  for(i=0; i<4; i++)
  {
    strcpy(forecast[i].hr, "--");
    strcpy(forecast[i].icon, "00x");
  }
}
/*******************************************************************************
* End of Clear_Forecast()
*******************************************************************************/ 

/*******************************************************************************
* Function Name: conv_date()
* Description: convert date ASCII to integer
*******************************************************************************/ 
static int conv_date(char *dt)
{
	int date;
	date = (dt[0] - 48) * 10;
	date += dt[1] - 48;
	return date;
}
/*******************************************************************************
* End of Clear_Forecast()
*******************************************************************************/  
/*static void readfile(char* filepath, char* fileContent)
{
    FILE *f;
    char c;
    int index;
    f = fopen(filepath, "rt");
    while((c = fgetc(f)) != EOF){
        fileContent[index] = c;
        index++;
    }
    fileContent[index] = '\0';
}*/
/******************************************************************************
*  END OF FILE: weat.c
******************************************************************************/

