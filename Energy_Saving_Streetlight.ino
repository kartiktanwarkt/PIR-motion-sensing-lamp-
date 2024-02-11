#include <SoftwareSerial.h>
#define RX 2 //RX reffer to the receiver signal
#define TX 3 //TX reffer to the Tranfer signal
const int SENSOR_PIN = 5;// the Arduino pin connected to output (middle) wire of sensor
const int RELAY_PIN = 4;// the Arduino pin which is connected to control relay
String AP = "tushar";     // wifi Name that will be Needed by Esp8266 to push Data
String PASS = "yahoo123"; // AP PASSWORD
String API = "SOQOJM05H891EL8H";   // Write API KEY
String HOST = "api.thingspeak.com"; //Site Name
String PORT = "80"; //most commonly used port numbers in the Transmission Control Protocol (TCP) suite
String field = "field1";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; //Bool variable
int valSensor = 1; //Integer variable
SoftwareSerial esp8266(RX,TX); 
 
  
void setup() {  //Void setup function to setup arduino board
  Serial.begin(9600); //"9600 baud" means that the serial port is capable of transferring a maximum of 9600 bits per second.
  esp8266.begin(115200);
  pinMode(SENSOR_PIN, INPUT);// Define SENSOR_PIN as Input from sensor
  pinMode(RELAY_PIN, OUTPUT);// Define RELAY_PIN as OUTPUT for relay
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK"); //Setting up the arduino to station mode
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK"); //AT+CWJAP ->  connect to an Access Point (like a router).
}

void loop() { //in the void loop you’ll write your main program, knowing that the initialization is already done
 valSensor = getSensorData(); // Calling a function by the name of getsensor data
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor); //Storing all data in a string 
sendCommand("AT+CIPMUX=1",5,"OK");//enable or disable multiple TCP Connections ,AT+CIPMUX =1 for Multiple connection
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK"); //establishing one of the three connections: TCP, UDP or SSL.
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">"); //Start sending data 
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK"); //closes TCP connection 
}

int getSensorData(){ //Function that is called in the upper section
  int motion =digitalRead(SENSOR_PIN);// read the sensor pin and stores it in "motion" variable

  // if motion is detected
  if(motion){
    Serial.println("Motion detected");
    digitalWrite(RELAY_PIN, HIGH);// Turn the relay ON
    return 200;
  // if motion is not detected
  }else{
     Serial.println("===nothing moves");   //To print on the  serial monitor "Noting moves" 
     digitalWrite(RELAY_PIN,LOW);// Turn the relay OFF
     return 0;
  }
  delay(500);  //Delaying for 0.5 second or 500 milliseconds
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
