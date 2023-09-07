
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int peakToPeak;
int Fclap = 1;
int Lclap = 1;
int count;
int start;
int clap = 1;
int finish;
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample; // "unsigned" means the number will always be positive, never negative.
int soundReading;

void setup() {
    myservo.attach(11);  // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);
    while (!Serial) {} // This is probably optional...
    pinMode(8, OUTPUT);//RED
    pinMode(10, OUTPUT);//GREEN
    pinMode(12, OUTPUT);//BLUE
    //motors
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(8, OUTPUT);
}

void loop() {
    soundReading = listen();
    if (soundReading >= 450 && count == 0) { // we'll only print the value if it's louder than the background noise

        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);
        count += 1;
        start = millis();
    } else if (soundReading > 450 && count >= 1) {
        digitalWrite(8, HIGH);
        delay(100); // You can experiment with this delay.
        digitalWrite(8, LOW);
        count = 0;
        finish = millis();
        float TimePassed = (finish - start);
        Serial.println(TimePassed);
        if (TimePassed > 200) {
            pos = 150;
        }
        for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
        }
    }

    if (peakToPeak > 350 && Fclap == 1) {
        digitalWrite(3, HIGH);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        digitalWrite(9, LOW);
        Fclap = 0;
    }
    if (Fclap == 35) {
        clap = 1;
    }

    if (peakToPeak > 350 && Lclap == 0) {
        digitalWrite(3, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(9, LOW);
        Fclap = 1;
    }

    if (Fclap == 1) {
        Lclap = 1;
    }

    if (Fclap == 0) {
        Lclap = 0;
    }
}
// This is the fancy function that performs a bit of "digital signal processing"
// to the raw data and then returns a value that corresponds to the volume of the sound.
int listen() {
    unsigned long startMillis = millis(); // Start of sample window
    float peakToPeak = 0;   // peak-to-peak level

    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // collect data for 50 ms
    while (millis() - startMillis < sampleWindow)
    {
        sample = analogRead(0);
        if (sample < 1024)  // toss out spurious readings
        {
            if (sample > signalMax)
            {
                signalMax = sample;  // save just the max levels
            }
            else if (sample < signalMin)
            {
                signalMin = sample;  // save just the min levels
            }
        }
    }

    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    delay(1);
    return peakToPeak;
}
