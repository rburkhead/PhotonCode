// Power pin 
int powerPin = D1; 

// Built-in LED 
int ledPin = D7; 

// Door sensor 
int doorSensorPin = D2; 
int doorSensorVal = LOW;
int doorPrevSensorVal = LOW; 
int doorOpenValue = LOW;

// PIR Sensor
int pirSensorPin = D0; 
int pirSensorVal = LOW;

// Time variable
unsigned long timeOn;
unsigned long timeOff;
#define WAIT_TIME_BTW_EVTS 6050

// Runs once when the device starts up. 
void setup() {
    // Turn the built-in LED Off
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    pinMode(doorSensorPin, INPUT_PULLDOWN); 
    
    // Turn power on to the door sensor. 
    pinMode(powerPin, OUTPUT); 
    digitalWrite(powerPin, HIGH);

    pinMode(pirSensorPin, INPUT_PULLDOWN);
}


// Runs often 
void loop() {
    time = millis();
    checkDoorSensor(); 
    checkPirSensor();
    delay(1000);
}


// Read the currnt value from the motion sensor and determine if we need to turn the light on. 
void checkPirSensor ()
{
    // Read the value from the PIR motion sensor.  
    pirSensorVal = digitalRead(pirSensorPin);

    // If the state of the door has changed and the door is open.
    if ((pirPrevSensorVal != pirSensorVal) && (pirPrevSensorVal == pirSensorVal))
    {
        changKitchenLightState(true);
    }
    
    // Save the previous state, so we can detect changes.  
    doorPrevSensorVal = doorSensorVal;
}

// Read the current door sensor value and determine if we need to turn the light on. 
void checkDoorSensor() {
    // Read the value from the door sensor. 
    doorSensorVal = digitalRead(doorSensorPin); 
    
    // If the state of the door has changed and the door is open.
    if ((doorPrevSensorVal != doorSensorVal) && (doorPrevSensorVal == doorOpenValue))
    {
        changKitchenLightState(true);
    }
    
    // Save the previous state, so we can detect changes.  
    doorPrevSensorVal = doorSensorVal;
}

// Change the state of the kitchen light. 
void changKitchenLightState (bool lightsOn) {
    if (lightsOn) // On 
    {
        digitalWrite(ledPin, HIGH);
        
        // publish the event that will trigger our Webhook
        Particle.publish("turn-kitchen-light-on", NULL, 60, PRIVATE);
    }
    else // Off  
    {
        digitalWrite(ledPin, LOW);
        
        // publish the event that will trigger our Webhook
        Particle.publish("turn-kitchen-light-off", NULL, 60, PRIVATE);
    }
}