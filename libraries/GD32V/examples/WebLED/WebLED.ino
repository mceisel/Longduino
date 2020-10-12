
const char line[] = "-----\n\r";
int loopCount=0;

char html[200];
char command[20];
char reply[500]; // you wouldn't normally do this

char ipAddress [20];
char LED[30];
int lenHtml = 0;
char temp[5];
int clientId = 0;

void get_res(int loops=24000000) {
  int cpos = 0;
  for(int cc=0; cc<loops; cc++) {
     while (Serial1.available()) {
       reply[cpos] = (char) Serial1.read(); cpos++;
     };
  };
  reply[cpos] = 0;
}

void get_res2(void) {
  get_res(500000);
}

size_t getReply(int wait, int maxlen = 500)
{ 
  get_res(wait*2000);
  return strlen(reply);
}

void setup() 
{
  Serial.begin(115200);  //serial port of GD32
  Serial1.begin(115200);  //serial port of ESP8266
  pinMode(LED_BUILTIN, OUTPUT);

  delay(3000);

  /* Disable echo of commands */
  Serial1.println("ATE0");
  getReply(2000);

  /* Disconnect from AP */
  Serial1.println("AT+CWQAP");
  getReply(2000);

  /* 1 = Station mode (client), 2 = AP mode (host), 3 = AP + Station mode */
  Serial1.println("AT+CWMODE=1");
  getReply(2000);

#if 0
  bool error = false;

  /* Set AP SSID, password, channel and encryption */
  Serial1.println("AT+CWSAP=\"esp_123\",\"1234test\",5,3");
  getReply(2000);

  /* Set AP IP */
  Serial1.println("AT+CIPAP=\"192.168.5.1\"");
  getReply(2000);

  strcpy(ipAddress,"192.168.5.1");

#else
  /* Connect to AP */
  Serial1.println("AT+CWJAP=\"Your WiFi SSID\",\"Password\""); // add your own wifi
  getReply(5000);

  /* Get local IP address */
  Serial1.print("AT+CIFSR\r\n");
  getReply(2000);

   int len = strlen( reply ); 
      bool done=false;
      bool error = false;
      int pos = 0;
      while (!done)
      {
           if ( reply[pos] == 34) { done = true;} 
           pos++;
           if (pos > len) { done = true;  error = true;}
      }

      if (!error)
      {
            int buffpos = 0;
            done = false;
            while (!done)
            {
               if ( reply[pos] == 34 ) { done = true; }
               else { ipAddress[buffpos] = reply[pos];    buffpos++; pos++;   }
            }
            ipAddress[buffpos] = 0;
      }
      else { strcpy(ipAddress,"ERROR"); }
#endif

      Serial.println(ipAddress);

      Serial1.print("AT+CIPMUX=1\r\n");
      getReply( 1500 );

      Serial1.print("AT+CIPSERVER=1,80\r\n");
      getReply( 1500 );

      if (!error)
          digitalWrite(LED_BUILTIN, HIGH);
}

void clientPrintBuf()
{
    lenHtml = strlen( html );
    strcpy(command,"AT+CIPSEND=");
    __itoa( clientId, temp, 10); strcat(command, temp);
    strcat(command,",");
    __itoa( lenHtml, temp, 10); strcat(command, temp);
    strcat(command, "\r\n");
    Serial1.print(command);
    getReply( 500 );
    Serial1.print(html);
    getReply( 1000 );
}

void clientPrintLn(const char* s)
{
    strcpy(html,s);
    strcat(html, "\r\n");
    clientPrintBuf();
}

void loop()
{
    get_res2();
    // check if the ESP8266 is sending data
    if ((int)strlen(reply) != 0) {
          // this is the +IPD reply - it is quite long. 
          // normally you would not need to copy the whole message in to a variable you can copy up to "HOST" only
          // or you can just search the data character by character as you read the serial port.

          size_t lenReply = strlen(reply);
          bool foundIPD = false;
          clientId = 0;

          if (lenReply > 6) {
              for (size_t i=0; i<(lenReply-3); i++)
              {
                  if (  (reply[i]=='I') && (reply[i+1]=='P') && (reply[i+2]=='D')   ) { foundIPD = true; }
              }
          }

          if ( foundIPD )
          {

              loopCount++;
              // Serial.print("Have a request.  Loop = ");  Serial.println(loopCount); Serial.println("");

              /* TODO: read client id from +IPD */

              bool LEDstate = false;
              size_t LEDstatepos = 0;
              if (lenReply > 6) {
                for (size_t i=0; i<(lenReply-3); i++)
                {
                   if (!LEDstate) // just get the first occurrence of name
                   {
                         if ( (reply[i]=='L') &&  (reply[i+1]=='E')&&  (reply[i+2]=='D')&&  (reply[i+3]=='=')) 
                         { 
                             LEDstate = true;
                             LEDstatepos = i+4;
                             break;
                         }

                   }     
                }
              }

              if (LEDstate)
              {
                    int tempPos = 0;
                    for (size_t i= LEDstatepos; i<lenReply; i++)
                    {
                         if ((reply[i]==' ') || (reply[i]=='\r') || (reply[i]=='\n')) { break; }
                         if (tempPos < 30) { LED[tempPos] = reply[i];   tempPos++; } else  { break; }
                    }
                    LED[tempPos] = 0;
              }

              if (LEDstate) { Serial.print("LED state = ");  Serial.println(LED); Serial.println("");

              } 
              else          { Serial.println("format incorrect");   Serial.println("");           }

              Serial.println("111");
              Serial.println(LED);
              Serial.println("000");
              if(strcmp(LED,"on")==0){digitalWrite(LED_BUILTIN, HIGH); }
              if(strcmp(LED ,"off")==0){digitalWrite(LED_BUILTIN, LOW); }

              strcpy(html,"HTTP/1.1 200 OK"); strcat(html, "\r\n");
              strcat(html,"Content-Type: text/html"); strcat(html, "\r\n");
              strcat(html,"Connection: close"); strcat(html, "\r\n"); // the connection will be closed after completion of the response
              strcat(html,""); strcat(html, "\r\n");
              strcat(html,"<!DOCTYPE HTML>"); strcat(html, "\r\n");
              clientPrintBuf();

              strcpy(html,"<html><head></head><body>"); strcat(html, "\r\n");

              strcat(html,"<h1>LED Test</h1>"); strcat(html, "\r\n");
              strcat(html,"<p>LED Statment</p>"); strcat(html, "\r\n");

              if (LEDstate)
              {
                  // write name
                  strcat(html,"<p>LED state is "); strcat(html, LED ); strcat(html,"</p>"); strcat(html, "\r\n");
              }

              clientPrintBuf();

              strcpy(html,"<form action=\""); strcat(html, "/"); strcat(html, "\" method=\"GET\">"); strcat(command, "\r\n");

              strcat(html,"LEDstate:<br><input type=\"text\" name=\"LED\">"); strcat(html, "\r\n");
              strcat(html,"<input type=\"submit\" value=\"Submit\"></form>"); strcat(html, "\r\n");

              strcat(html,"</body></html>"); strcat(html, "\r\n");
              strcat(html,""); strcat(html, "\r\n");
              clientPrintBuf();

              strcpy(command,"AT+CIPCLOSE=");
              __itoa( clientId, temp, 10); strcat(command, temp);
              strcat(command, "\r\n");
              Serial1.print(command);

              getReply( 1500 );

          } // if(espSerial.find("+IPD"))
    }

    for (int i=0; i<3 ;i++) { LED[i]=0; }
    delay (1);

    // drop to here and wait for next request.
}
