#include <SPI.h>
#include <SD.h>
#define SD_ChipSelectPin 10  //using digital pin 10 to connect to sd card
#include <TMRpcm.h>           //  also need to include this library...

TMRpcm tmrpcm;   // create an object for use in this sketch

          /*constants
          const int failureButtonPin = ;     // the number of the failure pushbutton pin
          const int winButtonPin = ;     // the number of the winning pushbutton pin
          const int redEarLeds =  ;      // the number of the sequence pin for failure
          const int headLeds =  ;      // the number of the sequence pin for winning
          */ 
          const int menueOne = 2;
          const int menueTwo = 3;
          const int menueThree = 4;
          // variables will change:
          int winButtonState = 0;             // variable for reading winning button status
          int failureButtonState = 0;         // variable for reading the failure pushbutton status
          int menueOneState = 0;
          int n = 0;
          
          void setup() {
            
            // initialize output pins:
            tmrpcm.speakerPin = 9;
            Serial.begin(9600);
            pinMode(A4, OUTPUT);
            pinMode(A3, OUTPUT);
            pinMode(A2, OUTPUT);
            pinMode(menueOne, OUTPUT);
            pinMode(menueTwo, OUTPUT);
            pinMode(menueThree, OUTPUT);
            
            // initialize the input pins:
            pinMode(A0, INPUT);
            pinMode(A1, INPUT);     
            
            
            //the following is code for the intro sequence 
            if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
              Serial.println("SD fail");  
              return;   // don't do anything more if not
            }
            
             tmrpcm.setVolume(4);
             Serial.println(SD.exists("s.wav"));
              Serial.print(tmrpcm.isPlaying());
              tmrpcm.play("s.wav"); //the sound file "music" will play each time the arduino powers up, or is reset
                Serial.print(tmrpcm.isPlaying()); 
             
            int c = 0;
            while (c < 3) {
              digitalWrite(A3, LOW);   //red ears
              digitalWrite(A2, HIGH);  //face
              digitalWrite(A4, HIGH); // yellow ears
              digitalWrite(menueOne, LOW);
              digitalWrite(menueTwo, LOW);
              digitalWrite(menueThree, LOW);
              delay(400);
              digitalWrite(menueOne, HIGH);
              digitalWrite(menueTwo, HIGH);
              digitalWrite(menueThree, HIGH);
              delay(400);
              digitalWrite(A3, HIGH);
              digitalWrite(A2, LOW);
               digitalWrite(A4,LOW); // yellow ears
              digitalWrite(menueOne, LOW);
              digitalWrite(menueTwo, LOW);
              digitalWrite(menueThree, LOW);
              delay(400);
              digitalWrite(menueOne, HIGH);
              digitalWrite(menueTwo, HIGH);
              digitalWrite(menueThree, HIGH);
              delay(400);
              c++;
            }
            
            
          }
        
        
          void loop(){
            if (tmrpcm.isPlaying()){}
            else{
              tmrpcm.play("p.wav");
            }
           
            // read the state of winning and losing circuts
            failureButtonState = digitalRead(A0);
            winButtonState = digitalRead(A1);
            
            
            //the following manipulates the probabability of getting an even 10 millis to create a makeshift timer
            if ( millis() % 10 == 0) {
                  n++;
                  //test sequence for menue animation
                  if (n / 3 == 70) {
                  digitalWrite(menueTwo, LOW);
                  digitalWrite(menueThree, LOW);
                  digitalWrite(menueOne, HIGH);
                  }
                  else if (n / 3 == 140){
                  digitalWrite(menueOne, LOW);
                  digitalWrite(menueThree, LOW);
                  digitalWrite(menueTwo, HIGH);
                  }
                  else if (n / 3 == 210) {
                    digitalWrite(menueThree, HIGH);
                    digitalWrite(menueTwo, LOW);
                    digitalWrite(menueOne, LOW);
                  }
                 if (n == 630) {
                   n = 0;
                 }
            }
            
            
            // check if losing circut is complete 
            if (failureButtonState == HIGH) {     
              // turn failure sequence on:    
              digitalWrite(A3, HIGH); 
              tmrpcm.play("2.wav"); 
              while(tmrpcm.isPlaying()){}
            } 
               else {
              // turn failure sequence off:
              digitalWrite(A3, LOW); 
            }
            
            //check if winning circut is complete
            if (winButtonState == HIGH) {
              // turn winning sequence on:
              tmrpcm.play("3.wav");
              int c = 0;
              while (c < 5) {
                digitalWrite(A3, LOW);
                digitalWrite(A2, HIGH);
                delay(800);
                digitalWrite(A2, LOW);
                digitalWrite(A3, HIGH);
                delay(800);
                c++;
              }
            }
            else {
              // turn winning sequence off:
              digitalWrite(A2, LOW);
            }
         
          }

