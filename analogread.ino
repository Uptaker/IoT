//int analogPin = A0; 
                  
float val = 0;  // variable to store the value read

void setup() {
  pinMode(16, OUTPUT); 
  Serial.begin(115200);           //  setup serial
}

void loop() {
  val = analogRead(A0);  
  Serial.println(val);   
  
  if(val<=421){
    digitalWrite(16, LOW); // turn the LED off by making the voltage LOW
  }else{
    digitalWrite(16, HIGH); // turn the LED on (HIGH is the voltage level)

      
  }
    
  


  
  delay(1000); // wait for a second

}
