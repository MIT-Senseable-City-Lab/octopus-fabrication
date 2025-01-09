#include "Octopus_Firmware.h"

unsigned long previousMillis = 0;
const long interval = 5000; // Interval in milliseconds
unsigned long blinkInterval = 100; // Blinking interval in milliseconds
unsigned long lastBlinkMillis = 0;
bool isBlinkOn = false;

// Button state variables
const int buttonPin = 7;  // Pin connected to the button
bool deviceOn = false; // Device state
bool longPressHandled = false; // To ensure long press is handled once
unsigned long buttonPressTime = 0;
const unsigned long longPressDuration = 2000; // Duration to consider as long press (2000ms)

const int RECORDS_PER_FILE = 10;
const int vbatPin = A0;         
const int chargeStatePin = 7;  

const float coldThreshold = 20.0; 
const float hotThreshold = 25.0;  

// Sensor availability flags
bool sps30Available = false;
bool gpsAvailable = false;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting Octopus Device...");

    // Initialize sensors
    Serial.println("Initializing sensors...");
    if (!Octopus::initializeSensors()) {
        Serial.println("Failed to initialize sensors. Continuing...");
    } else {
        Serial.println("Sensors initialized.");
    }
  // Attempt to initialize GPS
    Serial.println("Initializing GPS...");
    gpsAvailable = Octopus::initializeGPS();
    if (!gpsAvailable) {
        Serial.println("GPS not detected.");
    } else {
        Serial.println("GPS initialized.");
    }

    // Attempt to initialize SPS30
    Serial.println("Initializing SPS30...");
    sps30Available = Octopus::initializeSPS30();
    if (!sps30Available) {
        Serial.println("SPS30 not detected.");
    } else {
        Serial.println("SPS30 initialized.");
    }

    // Continue startup even if sensors failed
    Serial.println("Starting data collection...");
    if (!Octopus::start()) {
        Serial.println("Failed to start data collection.");
    } else {
        Serial.println("Data collection started.");
    }
 // Initialize SD card
    Serial.println("Initializing SD card...");
    initSD(RECORDS_PER_FILE);
    Serial.println("SD card initialized.");

    initBatteryMonitoring();

    pinMode(buttonPin, INPUT_PULLUP);
    Serial.println("Setup complete.");
}

void loop() {
    unsigned long currentMillis = millis();

    int buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
        if (buttonPressTime == 0) {
            buttonPressTime = millis();
        }

        if ((millis() - buttonPressTime) >= longPressDuration) {
            if (!longPressHandled) {
                deviceOn = false;
                Serial.println("Device turned off");
                setDotStarColor(0, 0, 0);
                if (sps30Available) {
                    Octopus::stopSPS30();
                }
                delay(100);
                longPressHandled = true;
            }
        }
    } else {
        if (buttonPressTime != 0) {
            if (!longPressHandled) {
                deviceOn = true;
                Serial.println("Device turned on");
                initSD(RECORDS_PER_FILE);
                initBatteryMonitoring();
                if (sps30Available) {
                    Octopus::initializeSPS30();
                }
            }
            buttonPressTime = 0;
            longPressHandled = false;
            delay(50);
        }
    }

    if (!deviceOn) {
        delay(100);
        return;
    }

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; 

        // Get GPS time if available
        String gpsTime = "N/A";
        if (gpsAvailable) {
            gpsTime = Octopus::getGPSTime();
        }

        // Read GPS data if available
        float latitude = 0, longitude = 0, altitude = 0;
        if (gpsAvailable) {
            if (!Octopus::readGPSData(latitude, longitude, altitude)) {
                Serial.println("Failed to read GPS data");
            }
        }

        // Read other sensor data
        float temperature = Octopus::readTemperature();
        float humidity = Octopus::readHumidity();

        // Read SPS30 data if available
        float pm1_0 = 0, pm2_5 = 0, pm4_0 = 0, pm10_0 = 0;
        if (sps30Available) {
            if (!Octopus::readSPS30Data(pm1_0, pm2_5, pm4_0, pm10_0)) {
                Serial.println("Failed to read SPS30 data");
            }
        }

        // Log the data (Include placeholders or skip the SPS30 values if not available)
        String data = gpsTime + "," + String(latitude, 7) + "," + String(longitude, 7) + "," + temperature + "," + humidity;
        if (sps30Available) {
            data += "," + String(pm1_0) + "," + String(pm2_5) + "," + String(pm4_0) + "," + String(pm10_0);
        } else {
            data += ",N/A,N/A,N/A,N/A";  // Placeholder if SPS30 data is unavailable
        }
        logToSD(data);

        // Print the data to the Serial monitor
        Serial.print("GPS Time: ");
        Serial.println(gpsTime);
        Serial.print("Latitude: ");
        Serial.println(latitude, 7);
        Serial.print("Longitude: ");
        Serial.println(longitude, 7);
      
        // Battery monitoring and RGB LED control
        int vbatRaw = analogRead(vbatPin);
        float vbatVoltage = vbatRaw * (3.294 / 1023.0) * 1.279;
        bool chargeState = digitalRead(chargeStatePin);
        bool batteryConnected = vbatVoltage > 2.5;
        float batteryPercentage = batteryConnected ? calculateBatteryPercentage(vbatVoltage) : 0.0;

        if (temperature < coldThreshold) {
            setDotStarColor(0, 0, 255); 
        } else {
            setDotStarColor(128, 0, 128); 
        }

        if (vbatVoltage < 2.5 || !batteryConnected) {
            if (currentMillis - lastBlinkMillis >= blinkInterval) {
                lastBlinkMillis = currentMillis;
                isBlinkOn = !isBlinkOn;
                if (isBlinkOn) {
                    setDotStarColor(255, 0, 0); 
                } else {
                    setDotStarColor(0, 0, 0);
                }
            }
        }

        Serial.print("VBAT Voltage: ");
        Serial.print(vbatVoltage, 2);
        Serial.print(" V, Charge State: ");
        Serial.print(chargeState ? "Charging" : "Not Charging");
        Serial.print(", Battery Percentage: ");
        Serial.print(batteryPercentage, 1);
        Serial.println(" %");

        Serial.println();
    }

    delay(100);
}
