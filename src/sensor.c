#include "sensor.h"


static size_t cb(char *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)clientp;
 
  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(!ptr)
    return 0;  /* out of memory! */
 
  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
 
  return realsize;
}

position_t stringToPos(char* text){
  if(strstr(text,"top")!=NULL) return top;
  if(strstr(text,"bottom")!=NULL) return bottom;
  if(strstr(text,"hide")!=NULL) return hide;
  return hide;
}

int updateSensor(haSensor* sensor, filepars* config){
  sensor->answer = calloc(555, sizeof(char));
  sensor->updateURL = malloc(strlen(sensor->path)+strlen(config->domain)+1);
  sensor->colorHIGH = malloc(strlen(config->critColor)+2); //length of hex color with #
  
  strcpy(sensor->colorHIGH, config->critColor);
  strcpy(sensor->updateURL, config->domain);
  strcat(sensor->updateURL, sensor->path);
  if(getData(sensor->updateURL,sensor->answer, config->token) == EXIT_FAILURE) return EXIT_FAILURE;
  getValue("state", sensor); 
  getUnit("unit_of_measurement",sensor);
  return EXIT_SUCCESS;
}


//gets raw Data from Homeassistant
//needs free o returned char*
int getData(char* data_url, char* response, char* token){
  struct memory chunk = {0};
  struct curl_slist *tokenHeader =  NULL;
  CURL *curl = curl_easy_init();
  char* tmp = malloc(strlen(token)*sizeof(char)+1);
  strcpy(tmp,token);
  tokenHeader = curl_slist_append(tokenHeader,tmp);
  tokenHeader = curl_slist_append(tokenHeader,"Content-Type: application/json");
  free(tmp);
  if(!curl) {
    fprintf(stderr, "init failed\n");
    response = NULL;
    return EXIT_FAILURE;
  }
   
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl, CURLOPT_URL, data_url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, tokenHeader);
  curl_easy_setopt(curl, CURLOPT_HEADEROPT, CURLHEADER_SEPARATE);
  //perform actions and print
  CURLcode result = curl_easy_perform(curl);
  int count = 0;
  while(result != CURLE_OK && count <= 20){
    fprintf(stderr, "Download Problem: %s\n", curl_easy_strerror(result));
    sleep(1);
    result = curl_easy_perform(curl);
    count ++;
}  
  if(result == CURLE_OK) {
    char *ct;
    result = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);

    response = strcpy(response, chunk.response);
  } else {
    curl_easy_cleanup(curl);
    curl_slist_free_all(tokenHeader);
    free(chunk.response);
    return EXIT_FAILURE;
  }
  curl_easy_cleanup(curl);
  curl_slist_free_all(tokenHeader);
  free(chunk.response);
  return EXIT_SUCCESS;
}

void getValue(char* search, haSensor* sensor){
  char* pos = strstr(sensor->answer, search) + strlen(search) + strlen("\":\"");
  char* end = strstr(pos,"\"");
  
  char* tmp = calloc(12,sizeof(char));
  memcpy(tmp,pos,end-pos);
  sensor->value = (float)atof(tmp);
  free(tmp);
}

void getUnit(char* search, haSensor* sensor){
  char* pos = strstr(sensor->answer, search) + strlen(search) + strlen("\":\"");
  char* end = strstr(pos,"\"");
  
  char* tmp = calloc(7,sizeof(char));
  memcpy(tmp,pos,end-pos);
  sensor->unit = calloc(strlen(tmp)+1,sizeof(char));
  strcpy(sensor->unit,tmp);
  free(tmp);
}

void printSens(haSensor * sensor, char* returnText){
  strcat(returnText,sensor->descr);
  if(sensor->value >= sensor->valueHIGH )  {
    strcat(returnText,"<span foreground='"); //add color for critical values
    strcat(returnText,sensor->colorHIGH); //add color for critical values
    strcat(returnText,"'>"); //add color for critical values
  }
  char *tmp = malloc(10*sizeof(char));
  sprintf(tmp,"%02.1f",sensor->value);
  strcat(returnText,tmp);
  free(tmp);
  if(sensor->value >= sensor->valueHIGH )  strcat(returnText,"</span>");
  strcat(returnText,sensor->unit);
}
