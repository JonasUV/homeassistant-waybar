#ifndef waybarHomeassistant
#define waybarHomeassistant
#include "sensor.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

static void initSensors(haSensor* [], int);
static void deinitSensors(haSensor* [], int, bool);
static void printBar(haSensor* [], int);

#endif // !waybarHomeassistant
