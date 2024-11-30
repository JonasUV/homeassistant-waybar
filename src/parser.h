#ifndef parser
#define parser
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sensor.h"

#define MAX_LINE_LENGTH 555

 
int readFile(char*,filepars*,haSensor*[]);
static int readParam(char*, char*,char*);
static int readHaParam(char*, haSensor*);
static int readHead(char*, char*);
int countSensors(char* filepath);
static bool isHead(char*);

#endif
