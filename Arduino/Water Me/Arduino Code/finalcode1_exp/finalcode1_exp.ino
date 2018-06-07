
String statusChWriteKey = "AIBGG8QEBB8D896T";


String canalID1 = "496303"; //  Actuator1 Channel ID 
//String canalID2 = "375599";//


#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
//#include <SimpleTimer.h>
#include <VarSpeedServo.h> 

SoftwareSerial EspSerial(6, 7); // Rx,  Tx
#define HARDWARE_RESET 8

VarSpeedServo myservo; 

const int soil_sensor = A0;  // Analog input pin that the soil moisture sensor is attached to
const int temp_sensor = A5;
const int light_sensor = A2;
const int LED = 10;
const int servoPin = 9; // the digital pin used for the servo
float moist,light,temp;
float steinhart;


#define FREEZE_LED 10

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

// Variables to be used with timers
long writeTimingSeconds = 17; // ==> Define Sample time in seconds to send data 16
long readTimingSeconds = 1; // ==> Define Sample time in seconds to receive data // changed from 5

long startWriteTiming = 0;
long elapsedWriteTime = 0;

long startReadTiming = 0;
long elapsedReadTime = 0;


int spare = 0;
boolean error;
boolean pump = 0; 
boolean lamp = 0; 


//Relays
#define ACTUATOR1 13 // RED LED   ==> Pump
#define ACTUATOR2 3 // GREEN LED ==> Lamp



void setup()
{
  Serial.begin(9600);
  
  pinMode(HARDWARE_RESET,OUTPUT);

  pinMode(ACTUATOR1,OUTPUT);
    
  digitalWrite(HARDWARE_RESET, HIGH);
  
  digitalWrite(ACTUATOR1, LOW); //LOW
  
  
 // DS18B20.begin();
  //dht.begin();

  EspSerial.begin(9600); // Comunicacao com Modulo WiFi
  EspHardwareReset(); //Reset do Modulo WiFi
  startWriteTiming = millis(); // starting the "program clock"


  
  pinMode(LED, OUTPUT);  // Set lepPin - 9 pin as an output
  pinMode(light_sensor, INPUT);// Set pResistor - A0 pin as an input (optional)
  pinMode(temp_sensor, INPUT);// Set pResistor - A0 pin as an input (optional)


  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(0,255,true); // set the intial position of the servo, as fast as possible, wait until done

  analogReference(EXTERNAL);

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
/*  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  //display.println(" Madgayrah");
  display.display();*/
  
}

void loop()
{
  /*
  start: //label 
  error=0;
  
  elapsedWriteTime = millis()-startWriteTiming; 
  
  if (elapsedWriteTime > (writeTimingSeconds*1000)) 
  {
    readSensors();

        // read the sensor:
    moist = analogRead(soil_sensor); 
    light = analogRead(light_sensor); 
    temp = analogRead(temp_sensor);          


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
    
    writeThingSpeak();
    startWriteTiming = millis();   
  }
  
  if (error==1) //Resend if transmission is not completed 
  {       
    Serial.println(" <<<< ERROR >>>>");
    delay (2000);  
    goto start; //go to label "start"
  }*/




  start: //label 
  error=0;
  
  elapsedWriteTime = millis()-startWriteTiming; 
  elapsedReadTime = millis()-startReadTiming; 

  if (elapsedReadTime > (readTimingSeconds*1000)) 
  {
    ESPcheck();//executar antes de qualquer leitura ou gravação
    int command = readThingSpeak(canalID1); 
    if (command != 9) pump = command; 
    //delay (5000);
  //  command = readThingSpeak(canalID2); 
   //if (command != 9) lamp = command; 

    takeActions();
    startReadTiming = millis();   
  }
  
  if (elapsedWriteTime > (writeTimingSeconds*1000)) 
  {
    ESPcheck();//executar antes de qualquer leitura ou gravação

            // read the sensor:
    moist = analogRead(soil_sensor); 
    light = analogRead(light_sensor); 
    temp = analogRead(temp_sensor);          


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
    
    writeThingSpeak();
    startWriteTiming = millis();   
  }
  
  if (error==1) //Resend if transmission is not completed 
  {       
    Serial.println(" <<<< ERROR >>>>");
    digitalWrite(FREEZE_LED, HIGH);
    delay (2000);  
    goto start; //go to label "start"
  }

   
}

/********* Conexao com TCP com Thingspeak *******/
void writeThingSpeak(void)
{

  startThingSpeakCmd();

  // preparacao da string GET
  String getStr = "GET /update?api_key=";
  
getStr += statusChWriteKey;
  getStr +="&field6=";
  getStr += String(moist);
  getStr +="&field7=";
  getStr += String(light);
  getStr +="&field3=";
  getStr += String(steinhart);
  getStr +="&field8=";
  getStr += String(spare);
  getStr += "\r\n\r\n";
  
  sendThingSpeakGetCmd(getStr); 
}

/********* Reset ESP *************/
void EspHardwareReset(void)
{
  Serial.println("Reseting......."); 
  digitalWrite(HARDWARE_RESET, LOW); 
  delay(500);
  digitalWrite(HARDWARE_RESET, HIGH);
  delay(8000);//Tempo necessário para começar a ler 
  Serial.println("RESET"); 
}

/********* Start communication with ThingSpeak*************/
void startThingSpeakCmd(void)
{
  EspSerial.flush();//limpa o buffer antes de começar a gravar
  
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // Endereco IP de api.thingspeak.com
  cmd += "\",80";
  EspSerial.println(cmd);
  Serial.print("revived ==> Start cmd: ");
  Serial.println(cmd);

  if(EspSerial.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

/********* send a GET cmd to ThingSpeak *************/
String sendThingSpeakGetCmd(String getStr)
{
  String cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  EspSerial.println(cmd);
  Serial.print("revive ==> lenght cmd: ");
  Serial.println(cmd);

  if(EspSerial.find((char *)">"))
  {
    EspSerial.print(getStr);
    Serial.print("revive ==> getStr: ");
    Serial.println(getStr);
    delay(500);//tempo para processar o GET, sem este delay apresenta busy no próximo comando

    String messageBody = "";
    while (EspSerial.available()) 
    {
      String line = EspSerial.readStringUntil('\n');
      if (line.length() == 1) 
      { //actual content starts after empty line (that has length 1)
        messageBody = EspSerial.readStringUntil('\n');
      }
    }
    Serial.print("MessageBody received: ");
    Serial.println(messageBody);
    return messageBody;
  }
  else
  {
    EspSerial.println("AT+CIPCLOSE");     // alert user
    Serial.println("ESP8266 CIPSEND ERROR: RESENDING"); //Resend...
    spare = spare + 1;
    error=1;
    return "error";
  } 
}


void takeActions(void)
{
  Serial.print("Pump: ");
  Serial.println(pump);
 // Serial.print("Lamp: ");
 // Serial.println(lamp);
  if (pump == 1) digitalWrite(ACTUATOR1, HIGH);
  else digitalWrite(ACTUATOR1, LOW);
 // if (lamp == 1) digitalWrite(ACTUATOR2, LOW);
//  else digitalWrite(ACTUATOR2, HIGH);
}


boolean ESPcheck(void)
{
  EspSerial.println("AT"); // Send "AT+" command to module
   
  if (echoFind("OK")) 
  {
    //Serial.println("ESP ok");
    digitalWrite(FREEZE_LED, LOW);
    return true; 
  }
  else //Freeze ou Busy
  {
    Serial.println("ESP Freeze ******************************************************");
    digitalWrite(FREEZE_LED, HIGH);
    EspHardwareReset();
    return false;  
  }
}


int readThingSpeak(String channelID)
{
  startThingSpeakCmd();
  int command;
  // preparacao da string GET
  String getStr = "GET /channels/";
  getStr += channelID;
  getStr +="/fields/1/last";
  getStr += "\r\n";

  String messageDown = sendThingSpeakGetCmd(getStr);
  if (messageDown[5] == 49)
  {
    command = messageDown[7]-48; 
    Serial.print("Command received: ");
    Serial.println(command);
  }
  else command = 9;
  return command;
}


boolean echoFind(String keyword)
{
 byte current_char = 0;
 byte keyword_length = keyword.length();
 long deadline = millis() + 5000; // Tempo de espera 5000ms
 while(millis() < deadline){
  if (EspSerial.available()){
    char ch = EspSerial.read();
    Serial.write(ch);
    if (ch == keyword[current_char])
      if (++current_char == keyword_length){
       Serial.println();
       return true;
    }
   }
  }
 return false; // Tempo de espera esgotado
}

