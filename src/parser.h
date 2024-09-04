#ifndef parser
#define parser
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sensor.h"

#define MAX_LINE_LENGTH 555

 
int readFile(char*,filepars*,haSensor*[]);
int readParam(char*, char*,char*);
int readHaParam(char*, haSensor*);
int readHead(char*, char*);
int countSensors(char* filepath);
bool isHead(char*);

#endif
