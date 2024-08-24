#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <BLEMidi.h>

Adafruit_MPU6050 mpu;

// Thresholds for determining drum hits
const int16_t threshold = 6; // Adjust as needed

// Set the desired MIDI notes for different drums
const uint8_t BASS_DRUM_NOTE = 36;  // MIDI note number for bass drum (C2)
const uint8_t SNARE_NOTE = 38;      // MIDI note number for snare
const uint8_t HIHAT_NOTE = 42;      // MIDI note number for hihat
const uint8_t CYMBAL_NOTE = 49;     // MIDI note number for cymbal

// Flags to track whether each drum sound has been played
bool bassDrumPlayed = false;
bool snarePlayed = false;
bool hihatPlayed = false;
bool cymbalPlayed = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Initialize the MPU6050 sensor
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(100);
    }
  }
  Serial.println("MPU6050 Found!");

  // Initialize BLE
  BLEMidiServer.begin("drums2222"); // Broadcasted name of the MIDI device
  Serial.println("Initializing bluetooth");
  Serial.println("Waiting for connections...");
}

void loop() {
  // Read accelerometer data
  sensors_event_t accelEvent;
  mpu.getAccelerometerSensor()->getEvent(&accelEvent);
  int16_t ax = accelEvent.acceleration.x;
  int16_t ay = accelEvent.acceleration.y;
  int16_t az = accelEvent.acceleration.z;

  // Print raw accelerometer readings
  Serial.print("Raw Accel (x,y,z): ");
  Serial.print(ax);
  Serial.print(", ");
  Serial.print(ay);
  Serial.print(", ");
  Serial.println(az);

  // Determine drum hits based on accelerometer readings
  if (ay >= threshold && !bassDrumPlayed) {
    playDrum(BASS_DRUM_NOTE);
    bassDrumPlayed = true;
    Serial.println("BASS DRUM");
  } else if (ay < threshold && bassDrumPlayed) {
    bassDrumPlayed = false;
  }

  if (ay <= -threshold && !snarePlayed) {
    playDrum(SNARE_NOTE);
    snarePlayed = true;
    Serial.println("SNARE");
  } else if (ay > -threshold && snarePlayed) {
    snarePlayed = false;
  }

  if (az >= threshold && !hihatPlayed) {
    playDrum(HIHAT_NOTE);
    hihatPlayed = true;
    Serial.println("HIHAT");
  } else if (az < threshold && hihatPlayed) {
    hihatPlayed = false;
  }

  if (az <= -threshold && !cymbalPlayed) {
    playDrum(CYMBAL_NOTE);
    cymbalPlayed = true;
    Serial.println("CYMBAL");
  } else if (az > -threshold && cymbalPlayed) {
    cymbalPlayed = false;
  }

  delay(100); // Adjust delay as needed for reading the accelerometer and sending MIDI messages
}

void playDrum(uint8_t note) {
  // Send a MIDI note on message
  BLEMidiServer.noteOn(0, note, 127);
  // Delay for a short duration to let the MIDI message be sent
  delay(10);
  // Send a MIDI note off message
  BLEMidiServer.noteOff(0, note, 0);
}
