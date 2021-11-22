#include<Servo.h>
Servo Myservo;
int pos;
void setup()
{
Myservo.attach(2);
}

void loop()
{
  
  
for(pos=0;pos<=180;pos++){
Myservo.write(pos);
delay(10);
}
  delay(100);
  
  for(pos=180;pos>=0;pos--){
Myservo.write(pos);
delay(10);
}
  delay(100);
  
}