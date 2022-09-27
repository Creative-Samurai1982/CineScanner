#include <MobaTools.h>
#include <avdweb_Switch.h>
#include <L298N.h>
int  rotationSpeed1;
char termChar = '\n';
bool rotResult = false;

const int dirPin1 = 4;    // Capstan
const int stepPin1 = 5;   // Capstan
const int dirPin2 = 6;    // Takeup
const int stepPin2 = 7;   // Takeup


#define MOTOR_STEPS 200   // Number of steps for a full rotation(360) // SPROCKET(CAPSTAN)
#define MOTOR_STEPS2 200  // Number of steps for a full rotation(360) // TAKEUP REEL
int MICROSTEPS = 16;     // 1 = Full Step, 2 = Half Step, 16 = 16th microstepping

const byte pushButtonpin = 8; //  Traction/Tension switch pin

MoToStepper stepper1(MOTOR_STEPS, STEPDIR);   // SPROCKET(CAPSTAN)
MoToStepper stepper2(MOTOR_STEPS2, STEPDIR);  // TAKEUP REEL

Switch Button01 = Switch(pushButtonpin);

void setup()
{
  Serial.begin(9600);

  stepper1.attach(stepPin1, dirPin1);  // SPROCKET(CAPSTAN)
  stepper2.attach(stepPin2, dirPin2);  // TAKEUP REEL
}

void loop()
{
  Button01.poll();
  if (Button01.switched()) Serial.print("Button01 switched ");
  if (Button01.pushed()) Serial.println("Button01 pushed "); //Serial.print(++i); Serial.println(" times");}
  if (Button01.released()) Serial.println("Button01 released");
    while(!Serial.available());
    String incommingString = Serial.readStringUntil('\n');
    int len;
    len = incommingString.length();
    Serial.print("incommingString = "); // for IDE to test
    Serial.println(incommingString);   // for IDE to test  
    String incommingStr = incommingString.substring(len-12,len); // truncate to the correct length
    
    if  (incommingStr.length() != 12)
    {
        rotResult = false;    // rotation was not successfull
    }
    else
    {   //input has correct length
        String mode = incommingStr.substring(0,1);
        String dir = incommingStr.substring(1,2);
        String noOfSteps = incommingStr.substring(2,6);
        String rotationSpeed = incommingStr.substring(6,10);
        String sdelay = incommingStr.substring(10,12);   
        int    rotationSpeed1 = rotationSpeed.toInt();
        int    noOfSteps1 = noOfSteps.toInt();
        int    sdelay1 = sdelay.toInt();
        int    microsteps;
     

        rotResult = true;  // initialize rotation result to erronous 
////////////////////////////////////////////////////////////////////////////////
  stepper1.setSpeed(rotationSpeed1 * 10);  // RPM // SPROCKET(CAPSTAN)
  stepper2.setSpeed(rotationSpeed1 * 2);  //RPM // TAKEUP REEL

        if (dir == "F") // Direction
          {
            stepper1.doSteps(noOfSteps1 * MICROSTEPS);        // SPROCKET(CAPSTAN)
            //stepper2.doSteps(noOfSteps1 * MICROSTEPS / 5 );  // TAKEUP REEL 255
            stepper2.doSteps(noOfSteps1 * MICROSTEPS );  // TAKEUP REEL 255
          }
        else
          {
            stepper1.doSteps(-noOfSteps1 * MICROSTEPS);        // SPROCKET(CAPSTAN)
            //stepper2.doSteps(-noOfSteps1 * MICROSTEPS / 5 );  // TAKEUP REEL 255
            stepper2.doSteps(-noOfSteps1 * MICROSTEPS);  // TAKEUP REEL 255
          }
    
        if (mode == "F") // Full Step Mode
        {
          MICROSTEPS=1;
          delay(1000);
          Serial.println(MICROSTEPS);
          Serial.println("Full Step");
          Serial.println("");
        }
        if (mode == "H") // Half Step Mode
        {
          MICROSTEPS=2;
          delay(1000);
          Serial.println(MICROSTEPS);
          Serial.println("Half Step");
          Serial.println("");
        }
        if (mode == "M") // 16th Microstepping Mode
        {
          MICROSTEPS=16;
          delay(1000);
          Serial.println(MICROSTEPS);
          Serial.println("16th Step");
          Serial.println("");
        }        
        rotResult = true;    // rotation was successfull                   
    }
      
    if (rotResult == true)
    {
        Serial.write("1111111111");  // rotation successfull
        Serial.println("");
    }
    else
    {
       Serial.write("0000000000");  // rotation failed
       Serial.println("");
    }  
}   // end pf script
