

const byte numChars = 13;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

bool rotResult = false;

String mode;
String dir;
int noOfSteps1;
int rotationSpeed1;
int sdelay1;

void setup() {
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
}

void loop() {
  recvWithEndMarker();
  storeData();
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void storeData()
{
  if (newData == true) {
    //while(!Serial.available());
    String incommingString = receivedChars;
    int len;
    len = incommingString.length();
    Serial.print("Sent String = "); // for IDE to test
    Serial.println(incommingString);   // for IDE to test
    String incommingStr = incommingString.substring(len - 12, len); // truncate to the correct length
    if  (incommingStr.length() != 12)
    {
      rotResult = false;    // rotation was not successfull
      newData = false;
      Serial.println("");
      Serial.println("FAILED");
      Serial.println("");
    }

    else
    { //input has correct length
      //String 
      mode = incommingStr.substring(0, 1);
      //String 
      dir = incommingStr.substring(1, 2);
      String noOfSteps = incommingStr.substring(2, 6);
      String rotationSpeed = incommingStr.substring(6, 10);
      String sdelay = incommingStr.substring(10, 12);
      //int    
      rotationSpeed1 = rotationSpeed.toInt();
      //int    
      noOfSteps1 = noOfSteps.toInt();
      //int    
      sdelay1 = sdelay.toInt();
      int    microsteps;
      
      // stepper1.setSpeed(rotationSpeed1 * 10);  // RPM // SPROCKET(CAPSTAN)
      // stepper2.setSpeed(rotationSpeed1 * 2);  //RPM // TAKEUP REEL
      rotResult = true;  // initialize rotation result to erronous
      newData = false;

      Serial.println("");
      Serial.println("Stored Data");
      Serial.println("===========");
      Serial.print("MODE  = ");
      Serial.println(mode);
      Serial.print("DIR   = ");
      Serial.println(dir);
      Serial.print("STEPS = ");
      Serial.println(noOfSteps1);
      Serial.print("SPEED = ");
      Serial.println(rotationSpeed1);
      Serial.print("DELAY = ");
      Serial.println(sdelay1);
      Serial.println("");
      /*
      Serial.println("DATA  STORED");
      Serial.println("SUCCESSFULLY");
      Serial.println("");
      */
      if (dir == "F")
        {
          Serial.println("Forward");
        }
        else
        {
          Serial.println("Backward");
        }
        
       if (mode == "F")
        {
          Serial.println("Full Step");
        }

       if (mode == "H")
        {
          Serial.println("Half Step");
        }

       if (mode == "M")
        {
          Serial.println("Micro Step");
        }
       rotResult = true;    // rotation was successfull
    }
    if (rotResult == true)
  {
    Serial.println("");
    Serial.write("1111111111");  // rotation successfull
    Serial.println("");
    Serial.println("----------------------------------------------");
  }
  else
  {
    Serial.println("");
    Serial.write("0000000000");  // rotation failed
    Serial.println("");
    Serial.println("----------------------------------------------");
  }
  }
}
