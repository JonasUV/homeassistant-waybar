#ifndef sensor_h
#define sensor_h
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>


typedef enum position_t { top = 3, bottom = 2, hide = 1 }position_t;

struct memory {
  char *response;
  size_t size;
};

typedef struct filepars{
  char *domain;
  char *token;
  char *critColor;
  int sCount;
}filepars;

typedef struct haSensor { 
  float value;
  int accuracy;
  float valueHIDE;
  float valueHIGH;
  char* colorHIGH;
  position_t  isTopBar;
  char* descr;
  char* unit;
  char* answer;
  char* path;
  char* updateURL;
  CURL* loader;
} haSensor;

position_t stringToPos(char*);
int getData(char*,char*,char*);
int updateSensor(haSensor*,filepars*);
static size_t cb(char *, size_t , size_t , void *);
void getValue(char*, haSensor*);
void getUnit(char*, haSensor*);
void printSens(haSensor*, char*);

#endif // !sensor_h
