#include "waybarHomeassistant.h"

int main(int argc, char* argv[]){
  if(argc != 2){
    fprintf(stderr,"Please enter filepath as argument\n");
    return EXIT_FAILURE;
  }
  //check if token has been changed
  filepars config;
  bool failed = false;
  memset(&config, 0, sizeof(filepars));
  config.domain = calloc(MAX_LINE_LENGTH, sizeof(char)+1);
  config.critColor = calloc(MAX_LINE_LENGTH, sizeof(char)+1);
  config.token  = calloc(MAX_LINE_LENGTH, sizeof(char)+1);
  memset(config.domain, 0, MAX_LINE_LENGTH);
  memset(config.token, 0, MAX_LINE_LENGTH);


  config.sCount = countSensors(argv[1]);
  if(config.sCount == -1){
    fprintf(stderr,"no sensors found in config");
    free(config.domain);
    free(config.critColor);
    free(config.token);
    config.domain = NULL;
    config.critColor = NULL;
    config.token = NULL;
    return EXIT_FAILURE;
  }

  haSensor* sensors[config.sCount];
  initSensors(sensors,config.sCount);

  if(readFile(argv[1],&config,sensors) == EXIT_FAILURE){
    fprintf(stderr,"Couldn't read fie\n");
    return EXIT_FAILURE;
  }

  if(strlen(config.token)<=44){
    fprintf(stderr, "Tokenlength invalid");
    return EXIT_FAILURE;
  }

  for(int i = 0; i <= config.sCount-1; i++){
    if(updateSensor(sensors[i],&config) == EXIT_FAILURE){
      printf("{\"text\": \"");
      printf("url or a sensor path invalid");
      printf("\", \"tooltip\": \"");
      printf("url %s could be wrong\\nor a snsor path is invalid\\nmake sure every sensor has a sensor. infront of it",config.domain);
      printf("\", \"class\": \"\"}\\0");
      failed = true;
      break;
    }
  }
  if(!failed)
    printBar(sensors, config.sCount);

  deinitSensors(sensors, config.sCount,failed);
  free(config.token);
  free(config.domain);
  free(config.critColor);
  config.token = NULL;
  config.domain = NULL;
  config.critColor = NULL;
  return EXIT_SUCCESS;
}

static void initSensors(haSensor* sens[], int size){
  for(int count = 0; count <= size-1;count++){
    sens[count]         = malloc(sizeof(haSensor));
    sens[count]->path      = calloc(155,sizeof(char));
    sens[count]->descr     = calloc(12,sizeof(char));


    //setting default values
    sens[count]->valueHIDE  = INT_MIN;
    sens[count]->valueHIGH  = INT_MAX;
    sens[count]->accuracy  = 2;
    sens[count]->colorHIGH  = "#ff000";
    sens[count]->isTopBar  = top;
  }
}

static void deinitSensors(haSensor* sens[], int size, bool failed){
  for(int count = 0; count <= size-1;count++){
    if(!failed){
      free(sens[count]->answer);
      free(sens[count]->updateURL);
      free(sens[count]->unit);
      free(sens[count]->colorHIGH);

      sens[count]->answer = NULL;
      sens[count]->updateURL = NULL;
      sens[count]->unit = NULL;
      sens[count]->colorHIGH = NULL;
    }
    free(sens[count]->descr);
    sens[count]->descr = NULL;
    free(sens[count]->path);
    sens[count]->path = NULL;
    free(sens[count]);
    sens[count] = NULL;
  }
}

static void printBar(haSensor* sensors[], int count){
  char* bartext = malloc(300*sizeof(char));
  *bartext = '\0';
  char* tooltipText = malloc(500*sizeof(char));
  *tooltipText = '\0';
  //tooltip sens needs to append \n
  
  for(int i = 0; i <= count-1; i ++){
    haSensor *tmp = sensors[i]; 
    if(tmp->valueHIDE > tmp ->value)
      tmp->isTopBar =  tmp->isTopBar -1;
    if(tmp->valueHIGH < tmp ->value && tmp->isTopBar +1 <= top)
      tmp->isTopBar =  tmp->isTopBar +1;

    switch(tmp->isTopBar){
      case top:
        printSens(tmp, bartext);
        strcat(bartext, " ");
        break;
      case bottom:
        printSens(tmp, tooltipText);
        strcat(tooltipText, "\\n");
        break;
      case hide:
        break;
      default:
        break;
    }
  }
  char *deleter = strlen(bartext) + bartext - strlen(" \0");
  strcpy(deleter,"\0");
  deleter = strlen(tooltipText) + tooltipText - strlen("\\n\0");
  strcpy(deleter,"\0");
  printf("{\"text\": \"");
  printf("%s",bartext);
  if(strlen(tooltipText) >=2){
    printf("\", \"tooltip\": \"");
    printf("%s",tooltipText);
  }
  printf("\", \"class\": \"\"}\\0");
  free(bartext);
  free(tooltipText);
  bartext = NULL;
  tooltipText = NULL;
}
