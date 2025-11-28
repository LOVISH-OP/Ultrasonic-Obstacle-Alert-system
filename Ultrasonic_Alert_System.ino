// Project: Ultrasonic Obstacle Detection and Alert System

// Description: Measures distance to an obstacle using HC-SR04 and provides 

//              tiered alerts via LEDs and a buzzer based on distance thresholds.


// -------- PIN SETUP --------

const int trigPin = 8;

const int echoPin = 9;


// LEDs

const int redLED = 2;    // too close (<= 10 cm)

const int yellowLED = 5; // medium (10 cm to 25 cm)

const int greenLED = 4;  // safe (> 25 cm)


// Buzzer

const int buzzerPin = 3;


void setup() {

  Serial.begin(9600);

  Serial.println("Ultrasonic Alert System Initialized.");


  // Set sensor pins

  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);


  // Set output pins

  pinMode(redLED, OUTPUT);

  pinMode(yellowLED, OUTPUT);

  pinMode(greenLED, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  

  // Initial state: turn everything off

  digitalWrite(redLED, LOW);

  digitalWrite(yellowLED, LOW);

  digitalWrite(greenLED, LOW);

  digitalWrite(buzzerPin, LOW);

}


// Function to calculate distance in centimeters (cm)

long getDistance() {

  // Clear the trigPin by setting it LOW for a moment

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);


  // Send a 10 microsecond pulse to trigger the sensor

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);


  // Measure the duration (Time-of-Flight) of the echo pulse

  // Timeout added (50000 µs) for stability if no object is found

  long duration = pulseIn(echoPin, HIGH, 50000); 


  // Check if a timeout occurred (duration is 0 if pulseIn times out)

  if (duration == 0) return -1;


  // Calculate the distance: Time * Speed_of_Sound / 2

  // Speed of sound is approx 0.034 cm/µs

  long distance = duration * 0.034 / 2; 

  return distance;

}


void loop() {

  long d = getDistance();


  Serial.print("Distance: ");

  Serial.print(d);

  Serial.println(" cm");


  // Reset all outputs before checking the new distance

  digitalWrite(redLED, LOW);

  digitalWrite(yellowLED, LOW);

  digitalWrite(greenLED, LOW);

  digitalWrite(buzzerPin, LOW);


  // If reading failed (d < 0), skip the logic for this cycle

  if (d < 0) {

    Serial.println("Reading failed (Timeout).");

    return;

  }


  // --- Alert Logic ---

  if (d <= 10) {

    // DANGER ZONE (<= 10 cm) → Red + Buzzer

    digitalWrite(redLED, HIGH);

    digitalWrite(buzzerPin, HIGH);

    Serial.println("ALERT: DANGER - CRITICAL PROXIMITY!");


  } else if (d > 10 && d <= 25) {

    // WARNING ZONE (10 cm to 25 cm) → Yellow

    digitalWrite(yellowLED, HIGH);

    Serial.println("Warning: Caution Range.");


  } else {

    // SAFE ZONE (> 25 cm) → Green

    digitalWrite(greenLED, HIGH);

    Serial.println("Safe: Clear Path.");

  }


  // Delay before the next reading

  delay(200);

}
