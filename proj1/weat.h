/*******************************************************************************
* File Name: weat.h
* Author: Manmohan
* Version: 1.0
* Date: 31/05/2018
* Description: Phrase & displaay weather forecast from http://openweathermap.org
*******************************************************************************/
#ifndef WEAT_H
#define WEAT_H

/*******************************************************************************
* MACRO DEFINITIONS
*******************************************************************************/
#define JSON_FILE_PATH "test.json"
#define BUFFER_SIZE 9800
#define MAX_TOKEN_COUNT 1400
#define DATE_CNT 3
#define HR_CNT 3
#define TEMP_CNT 5
#define HUM_CNT 5
#define DESC_CNT 25
#define ICON_CNT 4
#define MAX_WEAT_DATA 30

/*******************************************************************************
*TYPE DEFINITIONS
*******************************************************************************/
typedef struct {
    char hr[HR_CNT]; //dt_txt
    char icon[ICON_CNT]; //weather.icon  
} forecast_t;

typedef struct {
    char date[DATE_CNT]; //dt_txt
    char hr[HR_CNT]; //dt_txt
    char temperature[TEMP_CNT]; //main.temp
    char humidity[HUM_CNT];//main.humidity
    char temp_max[TEMP_CNT]; //main.temp_max
    char icon[ICON_CNT]; //weather.icon  
    char temp_min[TEMP_CNT];//main.temp_min
    char Descr[DESC_CNT]; //weather.description
} weather_data;

/*******************************************************************************
*INTERFACE APIS
*******************************************************************************/
extern void weat_main(void);
#endif
/******************************************************************************
*  END OF FILE: weat.h
******************************************************************************/
