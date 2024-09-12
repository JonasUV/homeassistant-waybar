#include "parser.h"

bool isHead(char* line){
    char *start,*end;

    start = strstr(line,"[");
    if(start != NULL){
      end   = strstr(start+1,"]");
      if(start != NULL && end != NULL)  return true;
    }
  return false;
}

int readHaParam(char* line, haSensor* sensor){
  int retState = -1;
  char* tmp = calloc(MAX_LINE_LENGTH,sizeof(char));
  if(readParam(line, sensor->path, "path") == EXIT_SUCCESS) retState = 1;
  if(readParam(line, sensor->descr, "descr") == EXIT_SUCCESS) retState = 1;
  if(readParam(line, tmp, "valueHIDE") == EXIT_SUCCESS) retState = 2;
  if(readParam(line, tmp, "valueHIGH") == EXIT_SUCCESS) retState = 3;
  if(readParam(line, tmp, "position") == EXIT_SUCCESS)  retState = 4;
  if(readParam(line, tmp, "accuracy") == EXIT_SUCCESS)  retState = 5;
  
  switch(retState){
    case 1: free(tmp);
            return EXIT_SUCCESS;
            break;
    case 2: sensor->valueHIDE = atof(tmp);
            free(tmp);
            return EXIT_SUCCESS;
            break;
    case 3: sensor->valueHIGH = atof(tmp);
            free(tmp);
            return EXIT_SUCCESS;
            break;
    case 4: sensor->isTopBar = stringToPos(tmp);
            free(tmp);
            return EXIT_SUCCESS;
            break;
    case 5: sensor->accuracy = atoi(tmp);
            free(tmp);
            return EXIT_SUCCESS;
            break;
    default:
      free(tmp);
      return EXIT_FAILURE;
  }
}

int readParam(char *line, char *hatoken,char* param){
  char* pos,*end;
  pos = strstr(line,param);
 
  if(pos == NULL)
    return EXIT_FAILURE;
  
  pos = strstr(pos,"\"");
  end = strstr(pos+1,"\"");
  if(pos == NULL || end == NULL)
    return EXIT_FAILURE;

  strncpy(hatoken,pos+sizeof(char),end-pos-sizeof(char));

  return EXIT_SUCCESS;
}


int countSensors(char* filepath){
  FILE* fp = fopen(filepath,"r");
  char *chunk = malloc(MAX_LINE_LENGTH*sizeof(char));
  
  if(fp == NULL){
    printf("Unable to open FILE %s\n",filepath);
    return EXIT_FAILURE;
  }

  int sCount = 0;
  
  while(fgets(chunk,MAX_LINE_LENGTH-1,fp) != NULL ){
      if(isHead(chunk)) sCount++;
  }
  sCount = sCount == 0 ? sCount - 1 : sCount;
  fclose(fp);
  free(chunk);
  return sCount;
}

int readFile(char *filepath,filepars* config,haSensor* sensor[]){
  FILE* fp = fopen(filepath,"r");
  char *chunk = malloc(MAX_LINE_LENGTH*sizeof(char));
  if(fp == NULL){
    printf("Unable to open FILE %s\n",filepath);
    return EXIT_FAILURE;
  }

  bool isDomain = false,isToken = false,isCritcolor = false;
  int iCount = -1;
  while(fgets(chunk,MAX_LINE_LENGTH-1,fp) != NULL ){
    if(!isDomain && readParam(chunk, config->domain,"haDomain") == EXIT_SUCCESS)      isDomain = true;
    if(!isToken && readParam(chunk, config->token,"haToken") == EXIT_SUCCESS)        isToken = true;
    if(!isCritcolor && readParam(chunk, config->critColor,"critColor") == EXIT_SUCCESS)  isCritcolor = true;
    if(isHead(chunk)) iCount++;
    if(iCount >= 0) readHaParam(chunk,sensor[iCount]);
    
  }
  strcat(config->domain,"/api/states/");
  fclose(fp);
  free(chunk);
  return EXIT_SUCCESS;
}


