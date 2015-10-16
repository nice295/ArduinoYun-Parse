#include <Parse.h>
#include <Bridge.h>

/***** Quickstart of Parse Arduino Yún SDK *****/
// https://www.parse.com/apps/quickstart#embedded/arduinoyun

/***** NOTE: ParseClient Parse is defined in SDK *****/
unsigned long time = 0;

void setup() {
  // Initialize Bridge
  Bridge.begin();
  
  // Initialize Serial
  Serial.begin(9600);
  
  while (!Serial); // wait for a serial connection
    Serial.println("Parse Starter Project");
  
  // Initialize Parse
  Parse.begin("1KVktcyptUaCadXvML2oV1t1DTKp47ktIWHalorf", "3e14zFUQiYkMPWhnMC8aPfAXZF5QWwPsNBiBq9bf");
  Serial.println("Parse inited.");

  // Start push service
  Parse.startPushService();
  Serial.print("Push Installation ID:");
  Serial.println(Parse.getInstallationId());
}

void wifiCheck() {
  Process wifiCheck;  // initialize a new process

  Serial.println();

  wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");  // command you want to run

  // while there's any characters coming back from the
  // process, print them to the serial monitor:
  while (wifiCheck.available() > 0) {
    char c = wifiCheck.read();
    Serial.print(c);
  }

  Serial.println();

}

void loop() {
   if (millis() > time + 10*1000) {
    wifiCheck();
    time = millis();

    ParseCloudFunction cloudFunction;
    cloudFunction.setFunctionName("event");
    ParseResponse response = cloudFunction.send();
    if (response.getErrorCode() == 0) {     
      Serial.print("Cloud code: ");
      Serial.println(response.getString("result"));
    }
    else {
      Serial.print("Cloud code error code: ");
      Serial.println(response.getErrorCode());
    }
    response.close();
   }
   
  // Check if there is a new push
  // A push with message {"alert":"A test push from Parse!"}
  // will turn on LED for 3 seconds
  if (Parse.pushAvailable()) {
    ParsePush push = Parse.nextPush();
    
    String message = push.getJSONBody();
    Serial.print("New push message size: ");
    Serial.println(message.length());
    Serial.print("New push message content: ");
    Serial.println(message);
    
    String command = push.getString("alert");
    if (command == "A test push from Parse!") {
     digitalWrite(13, HIGH); // turn on LED
     delay(3000);  // wait 3 seconds
     digitalWrite(13, LOW); // turn off LED
    }
    // NOTE: ensure to close current push message
    // otherwise next push won't be available
    push.close();
  }

  Serial.flush();
}
