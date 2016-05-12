// Power pin used for the PIR sensor
int powerPin = D1; 

// Built-in LED 
int ledPin = D7; 

// Door sensor signal pin
int doorSensorPin = D2; 

// PIR Sensor signal pin 
int pirSensorPin = D0; 

// Time variable
int timeKitchenWebhookTriggerSent_On = 0; 
int timeKitchenWebhookTriggerSent_Off = 0; 


#define TIME_BTW_WEBHOOK_TRIGGERS 7 // A user by default may trigger a hook up to 10 times per minute for every device that is registered to their account
#define TIME_TO_LEAVE_LIGHT_ON 10 // The amount of time to leave the light on after it's been turned on. 
#define PIR_VALUE_WHEN_MOTION_DETECTED HIGH
#define DOOR_VALUE_WHEN_OPEN LOW

// Runs once when the device starts up. 
void setup() {
    // Turn the built-in LED Off
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // Set door sensor to INPUT. 
    pinMode(doorSensorPin, INPUT_PULLDOWN); 
    
    // Turn power on to the door sensor. 
    pinMode(powerPin, OUTPUT); 
    digitalWrite(powerPin, HIGH);

    // Set PIR pin to INPUT. 
    pinMode(pirSensorPin, INPUT_PULLDOWN);
}


// Runs often 
void loop() {
    checkDoorSensor(); 
    checkPirSensor();
    delay(1000);
}


// Read the currnt value from the motion sensor and determine if we need to turn the light on. 
void checkPirSensor (){
    // Read the value from the PIR motion sensor.  
    changKitchenLightState (digitalRead(pirSensorPin) == PIR_VALUE_WHEN_MOTION_DETECTED);
}

// Read the current door sensor value and determine if we need to turn the light on. 
void checkDoorSensor() {
    // Read the value from the door sensor. 
    changKitchenLightState (digitalRead(doorSensorPin) == DOOR_VALUE_WHEN_OPEN);
}

// Change the state of the kitchen light.
// Publishs the appropriate particle event to trigger the lights to change state. 
void changKitchenLightState (bool lightsOn) {
    if (lightsOn)
    {
        if (hasEnoughTimePassedToPublishWebhookTrigger (timeKitchenWebhookTriggerSent_On))
        {
            digitalWrite(ledPin, HIGH);

            // publish the event that will trigger our Webhook
            Particle.publish("turn-kitchen-light-on", NULL, 60, PRIVATE);
            timeKitchenWebhookTriggerSent_On = Time.now();
        }
        else if (lightsOn == false && 
                 hasEnoughTimePassedToTurnLightOff(timeKitchenWebhookTriggerSent_On) && 
                 hasEnoughTimePassedToPublishWebhookTrigger (timeKitchenWebhookTriggerSent_Off)) // Off  
        {
            digitalWrite(ledPin, LOW);
            // publish the event that will trigger our Webhook
            Particle.publish("turn-kitchen-light-off", NULL, 60, PRIVATE);
            timeKitchenWebhookTriggerSent_Off = Time.now();
        }
    }
}

// Once the lights come on they should stay on for set amount of time. 
// This function will tell the caller whether or not enough time has passed to turn the lights off. 
bool hasEnoughTimePassedToTurnLightOff (int lastTimeLightWasTurnedOn)
{
    return ((Time.now() - lastTimeLightWasTurnedOn) > TIME_TO_LEAVE_LIGHT_ON); 
}

// A user by default may trigger a webook up to 10 times per minute for every device that is registered to their account.
// This function will tell the caller whether or not enough time has passed to trigger a webhook. 
bool hasEnoughTimePassedToPublishWebhookTrigger  (int timeSinceLastTriggerWasPublished)
{
    return ((Time.now() - timeSinceLastTriggerWasPublished) > TIME_BTW_WEBHOOK_TRIGGERS); 
}