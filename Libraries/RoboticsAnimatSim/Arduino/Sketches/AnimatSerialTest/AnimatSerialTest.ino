#include "AnimatSerial.h"

AnimatSerial animat(&Serial, 10, 10);

#define PI 3.14159
#define PI_2 2*3.14159

float x=0;
int counter=0;
int counterror=0;
AnimatData data;

void setup() {
  //Serial.begin(57600);
  //while(!Serial);
  //Serial.println("Starting setup");

  animat.begin(38400);  // 115200
}

void loop() {
  
  animat.readMsgs();

  if(animat.isChanged())
  {
    for(int i=0; i<animat.getInDataTotal(); i++)
    {
      if(animat.isChanged(i) && animat.getData(i, data))
      {
        //Serial.print("Received Data ID: ");
        //Serial.print(i);
        //Serial.print(", val: ");
        //Serial.println(data.value.fval, 8);
      }
    }
    
    animat.clearChanged();
  }

  counter++;
  if(counter == 200) {
    counterror++;
    if(counterror==100)
    {
      //animat.writeResendMsg();
      counterror=0;
    }
    
    float val1 = 10*sin(x), val2=0;

    x += 0.01;
    if(x > PI_2)
    {
      val2=1;
      //Serial.println("Pulsing Val2");
    }
      
    //Serial.print("Adding data: ");
    //Serial.println(val1);
    animat.addData(0, val1);
    animat.addData(1, val2);
  
    //Serial.println("Writing data");
    animat.writeMsgs();
    
    if(x > PI_2)
      x = 0;
      
    counter = 0;
  }
}
