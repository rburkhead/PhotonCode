// Power pin 
int powerPin = D1; 

// Built-in LED 
int ledPin = D7; 

// Door sensor 
int doorSensorPin = D2; 
int doorSensorVal = HIGH;
int doorOpenValue = LOW;

// PIR Sensor
int pirSensorPin = D0; 
int pirSensorVal = LOW;
int pirMotionValue = HIGH;




#define TIME_BTW_WEBHOOK_TRIGGERS 7 // A user by default may trigger a hook up to 10 times per minute for every device that is registered to their account
#define TIME_TO_LEAVE_LIGHT_ON 10 // The amount of time to leave the light on after it's been turned on. 


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

    changKitchenLightState (pirSensorVal == pirMotionValue);
}

// Read the current door sensor value and determine if we need to turn the light on. 
void checkDoorSensor() {
    // Read the value from the door sensor. 
    doorSensorVal = digitalRead(doorSensorPin); 

    changKitchenLightState (doorSensorVal == doorOpenValue);
}

// Time variable
int timeKitchenWebhookTriggerSent_On = 0;
int timeKitchenWebhookTriggerSent_Off = 0;


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

bool isKitchenLightOn (){
    return false; 
}

bool hasEnoughTimePassedToTurnLightOff (int lastTimeLightWasTurnedOn)
{
    return ((Time.now() - lastTimeLightWasTurnedOn) > TIME_TO_LEAVE_LIGHT_ON); 
}

bool hasEnoughTimePassedToSendWebhookTrigger (int timeSinceLastTrigger)
{
    return ((Time.now() - timeSinceLastTrigger) > TIME_BTW_WEBHOOK_TRIGGERS); 
}