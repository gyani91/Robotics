#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SimpleTimer.h>
#include <VarSpeedServo.h> 
 
VarSpeedServo myservo; 
SimpleTimer timer;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 


const int soil_sensor = A0;  // Analog input pin that the soil moisture sensor is attached to
const int temp_sensor = A3;
const int light_sensor = A2;
const int LED = 13;
const int servoPin = 9; // the digital pin used for the servo
float moist,light,temp;


#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    

#define aref_voltage 3.3 

uint16_t samples[NUMSAMPLES];


void repeatMe() {
    Serial.print("Uptime (s): ");
    Serial.println(millis() / 1000);
}


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  timer.setInterval(1000, repeatMe);

  pinMode(LED, OUTPUT);  // Set lepPin - 9 pin as an output
  pinMode(light_sensor, INPUT);// Set pResistor - A0 pin as an input (optional)
  pinMode(temp_sensor, INPUT);// Set pResistor - A0 pin as an input (optional)

  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(0,255,true); // set the intial position of the servo, as fast as possible, wait until done

  analogReference(EXTERNAL);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  //display.println(" Madgayrah");
  display.display();
}


void loop() {
  // read the sensor:
    moist = analogRead(soil_sensor); 
    light = analogRead(light_sensor); 
    temp = analogRead(temp_sensor);          

  //float mv = (temp/1024.0)*5000; 
  //float cel = mv/10;

  //float voltage = temp * aref_voltage;
  //voltage /= 1024.0; 
  //float temperatureC = (voltage - 0.5) * 100 ;
  //temp = temp * 0.48828125;
  // print the sensor results to the serial monitor:
  //Serial.print("Moisture Value = " );                       
  //Serial.println(moist);  



  uint8_t i;
  float average;
 
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(temp_sensor);
   delay(10);
  }



  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
 
  Serial.print("Average analog reading "); 
  Serial.println(average);
 
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);
 
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
 
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
  
  
  Serial.print("light Value = " );                       
  Serial.println(light); 

  Serial.print("Moist Value = " );                       
  Serial.println(moist); 

   display.clearDisplay();
   display.setTextColor(WHITE);
   display.setCursor(0,0);


   display.println("       Water Me!");
   display.println("");
   display.print("Moisture: ");
   display.setTextSize(1.75);
   display.println(moist);

   display.print("Light: ");
   display.setTextSize(1.75);
   display.println(light);

   display.print("Temperature: ");
   display.setTextSize(1.75);
   display.print(steinhart);
   
  // delay(0000);
   //if(moist<400){
   // display.clearDisplay();
    //display.setTextSize(2);
    //display.println("Watering");

   //myservo.write(40,255,true);        // move the servo to 180, max speed, wait until done
   //delay(1000);                                   // write(degrees 0-180, speed 1-255, wait to complete true-false)
   //myservo.write(0,30,true);           // move the servo to 180, slow speed, wait until done
   //}

   //else{
    //digitalWrite(LED, LOW);
    //display.clearDisplay();
   //}
     if(moist<500){
    display.clearDisplay();
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    display.setTextSize(1);
    display.print("    Water me Dude");
    delay(5000);
   //myservo.write(40,255,true);        // move the servo to 180, max speed, wait until done
   //delay(4000);                                   // write(degrees 0-180, speed 1-255, wait to complete true-false)
   //myservo.write(0,30,true);           // move the servo to 180, slow speed, wait until done
   //delay(3000);
   }

   else{
    digitalWrite(LED, LOW);
    //display.clearDisplay();
   }


   display.display();                    
}

