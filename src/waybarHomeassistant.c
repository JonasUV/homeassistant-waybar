#include "waybarHomeassistant.h"

int main(int argc, char* argv[]){
  if(argc != 2){
    fprintf(stderr,"Please enter filepath as argument");
    return EXIT_FAILURE;
  }
  //check if token has been changed
  filepars config;
  memset(&config, 0, sizeof(filepars));
  config.domain = calloc(MAX_LINE_LENGTH, sizeof(char)+1);
  config.critColor = calloc(MAX_LINE_LENGTH, sizeof(char)+1);
  config.token  = calloc(MAX_LINE_LENGTH, sizeof(char)+1);
  memset(config.domain, 0, MAX_LINE_LENGTH);
  memset(config.token, 0, MAX_LINE_LENGTH);


  config.sCount = countSensors(argv[1]);
  if(config.sCount == -1){
    fprintf(stderr,"no sensors found in config");
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
      printf("ERRRR");
      printf("\", \"class\": \"\"}\\0");
    }
  }

  printBar(sensors, config.sCount);

  deinitSensors(sensors, config.sCount);
  free(config.token);
  free(config.domain);
  free(config.critColor);
  return EXIT_SUCCESS;
}

void initSensors(haSensor* sens[], int size){
  for(int count = 0; count <= size-1;count++){
    sens[count]         = malloc(sizeof(haSensor));
    sens[count]->path      = calloc(155,sizeof(char));
    sens[count]->descr     = calloc(12,sizeof(char));
  }
}

void deinitSensors(haSensor* sens[], int size){
  for(int count = 0; count <= size-1;count++){
    free(sens[count]->answer);
    free(sens[count]->updateURL);
    free(sens[count]->unit);
    free(sens[count]->colorHIGH);
    free(sens[count]->descr);
    free(sens[count]->path);
    free(sens[count]);
  }
}

void printBar(haSensor* sensors[], int count){
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
}