#ifndef waybarHomeassistant
#define waybarHomeassistant
#include "sensor.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

void initSensors(haSensor* [], int);
void deinitSensors(haSensor* [], int);
void printBar(haSensor* [], int);

#endif // !waybarHomeassistant
