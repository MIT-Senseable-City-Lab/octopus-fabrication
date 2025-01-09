# Octopus Firmware Library

The Octopus Firmware Library provides easy-to-use functions and examples to help you get started with the Arduino Nano 33 BLE Sense and Nicla Vision boards. Whether you're a beginner or an experienced maker, this library offers a simple starting point and advanced features to expand your projects.

## Quick Installation

### Step 1: Install the Library via Arduino Library Manager
1. Open the Arduino IDE.
2. Go to `Tools` > `Manage Libraries..`.
3. Search for **Octopus Firmware** and click `Install`.

### Step 2: Install Required Dependencies
When prompted to install the required dependencies, select "Install all." (Note: If you have previously installed the necessary dependencies, this prompt may not appear.)
Install these additional libraries from the Arduino Library Manager:
- **Adafruit_DotStar**
- **sensirion-sps**
- **Arduino_HS300x**
- **ArduinoBLE**
- **SD**
- **SparkFun_u-blox_GNSS_Arduino_Library**

### Firmware Usage Guide
### Example: Reading Temperature and Humidity

Here's a simple code snippet to demonstrate reading both temperature and humidity using the Octopus Firmware Library:

```cpp
#include "octopus.h"

void setup() {
    Serial.begin(9600);
    Octopus::initializeSensors();  // Initialize the sensors
}

void loop() {
    float temperature = Octopus::readTemperature();  // Read temperature
    float humidity = Octopus::readHumidity();        // Read humidity
    
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    delay(1000); // Read temperature and humidity every second
}
```
## Main Functions and Usage

### 1. Initializing Sensors and GPS
- **Initialize Sensors**: Use `Octopus::initializeSensors()` to initialize all connected sensors.
- **Initialize GPS**: Use `Octopus::initializeGPS()` to start the GPS functionality and prepare it for data retrieval.

### 2. Reading Temperature and Humidity
- **Read Temperature**: Use `Octopus::readTemperature()` to get the current temperature in degrees Celsius.
- **Read Humidity**: Use `Octopus::readHumidity()` to obtain the current humidity percentage.

### 3. Managing Device State with Button Press
- **Button Control**: Use a button to toggle the device state between on and off.
  - A **short press** turns the device on.
  - A **long press** (2 seconds) turns the device off.

### 4. Reading and Logging GPS Data
- **Retrieve GPS Time**: Use `Octopus::getGPSTime()` to get the current GPS time as a string.
- **Read GPS Coordinates**: Use `Octopus::readGPSData()` to read GPS coordinates (latitude, longitude) and altitude.

### 5. Reading Particulate Matter Data (SPS30)
- **Read Particulate Matter**: If the SPS30 sensor is available, use `Octopus::readSPS30Data()` to get particulate matter readings (PM1.0, PM2.5, PM4.0, PM10.0).



