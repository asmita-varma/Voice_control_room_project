
/***************PIN VARIABLES*******************/
int LDR = A0; //LDR sensor pin analog A0
int LM = A1;  //LM-35 sensor pin analog A1
int HC=13;  //Bluetooth module HC-05 power pin digital 13
int Mode=2; //Pin for LED of auto and manual mode
int LIGHT = 12; //Pin to operate relay of Light
int AC = 11;  //pin to operate relay of AC
/***********************************************/

/******************VARIABLES********************/
String Voice_Command="";
int pre_state = 0;
float LDR_in;
int LM_in;
int TEMP_Cel;
/**********************************************/

void setup() 
{
  
  Serial.begin(9600); //Setting up the Serial monitor with baud rate of 9600
/*******************PIN MODE SETTING****************/
  pinMode(LDR,INPUT);
  pinMode(LM,INPUT);
  pinMode(HC,OUTPUT);
  pinMode(Mode,OUTPUT);
  pinMode(LIGHT,OUTPUT);
  pinMode(AC,OUTPUT);
/****************************************************/

//Optimzation Code Due To Wrong Circuitary
  digitalWrite(LIGHT,HIGH); // Turning the relay for light off
  digitalWrite(AC,HIGH);  //Turning the relay for AC off
  
  digitalWrite(HC,HIGH); // continuous 5V Power supply for HC-05 bluetooth module  

//Serial.println("running");
  
}

void loop() {

    Voice_Input(); // Taking voice command input through Bluetooth from user
   
   /*******************SET MANUAL MODE**********************/
   if(Voice_Command == "*manual")
   {
//Voice command to set system to manual mode   
    pre_state = 0;
    digitalWrite(Mode,LOW);
   }
   /*******************************************************/

   
   /***********AUTOMATIC MODE**********************/
   else if((Voice_Command == "*auto")||(pre_state == 1))
   {
//    Voice command to set system to automatic mode
    pre_state = 1;
    digitalWrite(Mode,HIGH); //Turning on the LED to represent auto mode
    
    Sensor_Reading(); //Reading the sensor values 
    
    if(TEMP_Cel>27) 
    {
//      Turning the AC on if temperature is greater than 27 celcius
      digitalWrite(AC,LOW);
    }
    else
    {
//      Turning AC OFF if temperature is less than 27 celcius
      digitalWrite(AC,HIGH);  
    }
    if(LDR_in < 32.5)
    {
//      Turning Light ON if room is dark
      digitalWrite(LIGHT,LOW);
    }
    else
    {
//      Turning Light OFF if room has good light
      digitalWrite(LIGHT,HIGH);  
    }
   }
   /************************************************/

   
   /***********MANUAL MODE COMMANDS*****************/
   else if(pre_state == 0)
   {
//    if code is on manual mode
    if(Voice_Command == "*light on")
    {
//      if voice command is "LIGHT ON" turn the relay for light on
      digitalWrite(LIGHT,LOW);
    }
    else if(Voice_Command == "*AC on")
    {
//      if voice command is "AC ON" turn the relay for AC on
      digitalWrite(AC,LOW); 
    } 
    else if(Voice_Command == "*light off")
    {
//      if voice command is "LIGHT OFF" turn the relay for light OFF
      digitalWrite(LIGHT,HIGH); 
    } 
    else if(Voice_Command == "*AC off")
    {
//      if voice command is "AC OFF" turn the relay for AC OFF
      digitalWrite(AC,HIGH); 
    }
   }  
   /************************************************/
   
  Voice_Command = ""; // Replacing the String variable again with NULL at the end
}


void Voice_Input()
{
  //Function for taking voice command input from user through Bluetooth module HC-05

    while(Serial.available())//Checking for availability of Bluetooth input on serial monitor
  {
    delay(10);
    char in = Serial.read(); //Reading the voice command character by character in "char in" variable
    
    if(in=='#') //checking for the terminator symbol in the command
     { break;} //breaking the loop if loop has reached terminator
    Voice_Command += in;  // Adding each character of the command to the String variable Voice_Command
  }
  
   //Serial.println(Voice_Command);
}

void Sensor_Reading()
{
    LDR_in = analogRead(LDR); // Accepting the readings of LDR for light intensity  
    LM_in = analogRead(LM); // Accepting the readings of LM-35 for the temperature of the room
    LM_in = ((LM_in/1024.0)*5000);  
    TEMP_Cel = LM_in/10;  // Converting the LM-35 reading into temperature in Celcius
    
    /*Serial.print("LDR : ");
    Serial.println(LDR_in);
    Serial.print("LM : ");
    Serial.println(TEMP_Cel);*/
}
