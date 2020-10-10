
const bool printReply = true;
const char line[] = "-----\n\r";
int loopCount=0;

char html[50];
char command[20];
char reply[500]; // you wouldn't normally do this

char ipAddress [20];
char LED[30];
int lenHtml = 0;
char temp[5];

void getReply(int wait)
{ 
    int tempPos = 0;
    uint64_t time = millis();
    while( (time + wait) > millis())
    {   
        while(Serial1.available()>0)
        {   
            char c = Serial1.read(); 
            if (tempPos < 500) { reply[tempPos] = c; tempPos++;   }
        }
        reply[tempPos] = 0;
    } 

    if (printReply) { Serial.println( reply );  Serial.println(line);     }
}
void setup() 
{
  Serial.begin(115200);  //serial port of GD32
  Serial1.begin(115200);  //serial port of ESP8266
  pinMode(LED_BUILTIN, OUTPUT);

  delay(3000);

  Serial1.println("AT+CWQAP");
  getReply(2000);

  Serial1.println("AT+CWMODE=1");
  getReply(2000);

  Serial1.println("AT+CWJAP=\"Your WiFi SSID\",\"Password\""); // add your own wifi
  getReply(5000);

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


      Serial.println(ipAddress);

      Serial1.print("AT+CIPMUX=1\r\n");
      getReply( 1500 );

      Serial1.print("AT+CIPSERVER=1,80\r\n");
      getReply( 1500 );


}

void loop()
{
          if(Serial1.available()) // check if the ESO8266 is sending data
        {
          // this is the +IPD reply - it is quite long. 
          // normally you would not need to copy the whole message in to a variable you can copy up to "HOST" only
          // or you can just search the data character by character as you read the serial port.

          getReply( 2000 );      


          bool foundIPD = false;
          for (size_t i=0; i<strlen(reply); i++)
          {
               if (  (reply[i]=='I') && (reply[i+1]=='P') && (reply[i+2]=='D')   ) { foundIPD = true;    }
          }




          if ( foundIPD  )  
          {

              loopCount++;
              // Serial.print( "Have a request.  Loop = ");  Serial.println(loopCount); Serial.println(""); 

            bool LEDstate = false;
            size_t LEDstatepos = 0;
              for (size_t i=0; i<strlen(reply); i++)
              {
                   if (!LEDstate) // just get the first occurrence of name
                   {
                         if ( (reply[i]=='L') &&  (reply[i+1]=='E')&&  (reply[i+2]=='D')&&  (reply[i+3]=='=')) 
                         { 
                             LEDstate = true;
                             LEDstatepos = i+4;

                         }

                   }     
              }

                  if (LEDstate)
              {
                    int tempPos = 0;
                    bool finishedCopying = false;
                    for (size_t i= LEDstatepos; i<strlen(reply); i++)
                    {
                         if ( (reply[i]==' ') && !finishedCopying )  { finishedCopying = true;   } 
                         if ( !finishedCopying )                     { LED[tempPos] = reply[i];   tempPos++; }
                    }              
                    //LED[tempPos] = 0;
              }

              if (LEDstate) { Serial.print( "LED state = ");  Serial.println(LED); Serial.println("");

              } 
              else          { Serial.println( "format incorrect");   Serial.println("");           }

              Serial.println("111");
              Serial.println(LED);
              Serial.println("000");
                  if(strcmp(LED,"on")==0){digitalWrite(LED_BUILTIN, HIGH); }
                  if(strcmp(LED ,"off")==0){digitalWrite(LED_BUILTIN, LOW); }
            strcpy(html,"<html><head></head><body>");
            strcpy(command,"AT+CIPSEND=0,25\r\n");
            Serial1.print(command);
            getReply(2000);
            Serial1.print(html);
            getReply(2000);


            strcpy(html,"<h1>LED Test</h1>");
            strcpy(command,"AT+CIPSEND=0,17\r\n");
            Serial1.print(command); 
            getReply(2000);      
            Serial1.print(html);
            getReply(2000);

            strcpy(html,"<p>LED Statment</p>");
            strcpy(command,"AT+CIPSEND=0,19\r\n");
            Serial1.print(command);  
            getReply(2000);     
            Serial1.print(html);
            getReply(2000);



                if (LEDstate)
             {
                  // write name
                  strcpy(html,"<p>LED state is "); strcat(html, LED ); strcat(html,"</p>");

                  // need the length of html for the cipsend command
                  lenHtml = strlen( html );
                  strcpy(command,"AT+CIPSEND=0,"); __itoa( lenHtml, temp, 10); strcat(command, temp); strcat(command, "\r\n");
                  Serial1.print(command);
                  getReply( 2000 );          
                  Serial1.print(html);
                  getReply( 2000 );                           
             }


              strcpy(html,"<form action=\""); strcat(html, ipAddress); strcat(html, "\" method=\"GET\">"); strcat(command, "\r\n");

              lenHtml = strlen( html );
              __itoa( lenHtml, temp, 10);
              strcpy(command,"AT+CIPSEND=0,"); 
              __itoa( lenHtml, temp, 10); 
              strcat(command, temp);  
              strcat(command, "\r\n");

              Serial1.print(command);
              getReply( 2000 );          
              Serial1.print(html);
              getReply( 2000 );          

              strcpy(html,"LEDstate:<br><input type=\"text\" name=\"LED\">");
              strcpy(command,"AT+CIPSEND=0,43\r\n");
              Serial1.print(command);
              getReply( 2000 );
              Serial1.print(html);         
              getReply( 2000 );         

              strcpy(html,"<input type=\"submit\" value=\"Submit\"></form>");
              strcpy(command,"AT+CIPSEND=0,43\r\n");
              Serial1.print(command);
              getReply( 2000 );       
              Serial1.print(html);
              getReply( 2000 );   



            strcpy(html,"</body></html>");
            strcpy(command,"AT+CIPSEND=0,14\r\n");
            Serial1.print(command);
            getReply( 2000 ); 
            Serial1.print(html);
            getReply( 2000 ); 

            Serial1.print( "AT+CIPCLOSE=0\r\n" );
            getReply( 1500 ); 

          } // if(espSerial.find("+IPD"))
      } //if(espSerial.available())

      for (int i=0; i<3 ;i++)
      {LED[i]=0;}
      delay (100);

      // drop to here and wait for next request.
}
