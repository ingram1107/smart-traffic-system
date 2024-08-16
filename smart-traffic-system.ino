#include <AceRoutine.h>

// vehicle traffic light
const int trafficTrigPin = 12;
const int trafficEchoPin = 11;
const int trafficRedLEDPin = 2;
const int trafficYellowLEDPin = 3;
const int trafficGreenLEDPin = 4;

// pedestrian traffic light
const int pedesTrigPin = 10;
const int pedesEchoPin = 9;
const int pedesRedLEDPin = 5;
const int pedesYellowLEDPin = 6;
const int pedesGreenLEDPin = 7;

float trafficDistance, trafficDuration, pedesDistance, pedesDuration;

// condition variables
bool isPedesGreen = false;

// trigger ultra sonic sensor
void triggerSensor(int pin) {
    // trigger pin initialised
    digitalWrite(pin, LOW);
    delayMicroseconds(2);

    // sending trigger
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
}

float durationToCM(float t) {
    return t * 0.034 / 2;
}

void initLED(int redLED, int yellowLED, int greenLED) {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
}

void lightUpGreenLED(int redLED, int greenLED) {
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
}

void lightUpYellowLED(int greenLED, int yellowLED) {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
}

COROUTINE(vehicleTrafficControl) {
    COROUTINE_LOOP() {
        initLED(trafficRedLEDPin,
                trafficYellowLEDPin,
                trafficGreenLEDPin);

        // trigger pin initialised
        digitalWrite(trafficTrigPin, LOW);
        COROUTINE_DELAY_MICROS(2);

        // sending trigger
        digitalWrite(trafficTrigPin, HIGH);
        COROUTINE_DELAY_MICROS(10);
        digitalWrite(trafficTrigPin, LOW);

        trafficDuration = pulseIn(trafficEchoPin, HIGH);
        trafficDistance = durationToCM(trafficDuration);
        Serial.print("Vehicle Distance: ");
        Serial.print(trafficDistance);
        Serial.println(" cm");

        while (trafficDistance <= 20.00 && !isPedesGreen) {
            lightUpGreenLED(trafficRedLEDPin,
                            trafficGreenLEDPin);

            // trigger pin initialised
            digitalWrite(trafficTrigPin, LOW);
            COROUTINE_DELAY_MICROS(2);

            // sending trigger
            digitalWrite(trafficTrigPin, HIGH);
            COROUTINE_DELAY_MICROS(10);
            digitalWrite(trafficTrigPin, LOW);

            trafficDuration = pulseIn(trafficEchoPin, HIGH);
            trafficDistance = durationToCM(trafficDuration);
            Serial.print("Vehicle Distance: ");
            Serial.print(trafficDistance);
            Serial.println(" cm");

            if (trafficDistance > 20.00 || isPedesGreen) {
                lightUpYellowLED(trafficGreenLEDPin,
                                 trafficYellowLEDPin);
                COROUTINE_DELAY(3000);
            }
        }

        COROUTINE_DELAY(250);
    }
}

COROUTINE(pedesTrafficControl) {
    COROUTINE_LOOP() {
        initLED(pedesRedLEDPin,
                pedesYellowLEDPin,
                pedesGreenLEDPin);

        // trigger pin initialised
        digitalWrite(pedesTrigPin, LOW);
        COROUTINE_DELAY_MICROS(2);

        // sending trigger
        digitalWrite(pedesTrigPin, HIGH);
        COROUTINE_DELAY_MICROS(10);
        digitalWrite(pedesTrigPin, LOW);

        pedesDuration = pulseIn(pedesEchoPin, HIGH);
        pedesDistance = durationToCM(pedesDuration);
        Serial.print("Pedestrian Distance: ");
        Serial.print(pedesDistance);
        Serial.println(" cm");

        while (pedesDistance <= 10.00) {
            lightUpGreenLED(pedesRedLEDPin,
                            pedesGreenLEDPin);
            isPedesGreen = true;

            // trigger pin initialised
            digitalWrite(pedesTrigPin, LOW);
            COROUTINE_DELAY_MICROS(2);

            // sending trigger
            digitalWrite(pedesTrigPin, HIGH);
            COROUTINE_DELAY_MICROS(10);
            digitalWrite(pedesTrigPin, LOW);

            pedesDuration = pulseIn(pedesEchoPin, HIGH);
            pedesDistance = durationToCM(pedesDuration);
            Serial.print("Pedestiran Distance: ");
            Serial.print(pedesDistance);
            Serial.println(" cm");

            if (pedesDistance > 10.00) {
                lightUpYellowLED(pedesGreenLEDPin,
                                 pedesYellowLEDPin);
                COROUTINE_DELAY(3000);
            }
        }

        isPedesGreen = false;
        COROUTINE_DELAY(250);
    }
}

void setup() {
    pinMode(trafficTrigPin, OUTPUT);
    pinMode(trafficEchoPin, INPUT);
    pinMode(trafficRedLEDPin, OUTPUT);
    pinMode(trafficYellowLEDPin, OUTPUT);
    pinMode(trafficGreenLEDPin, OUTPUT);

    pinMode(pedesTrigPin, OUTPUT);
    pinMode(pedesEchoPin, INPUT);
    pinMode(pedesRedLEDPin, OUTPUT);
    pinMode(pedesYellowLEDPin, OUTPUT);
    pinMode(pedesGreenLEDPin, OUTPUT);

    Serial.begin(9600);
}

void loop() {
    vehicleTrafficControl.runCoroutine();
    pedesTrafficControl.runCoroutine();
}
