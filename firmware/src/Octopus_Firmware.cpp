#include "Octopus_Firmware.h"

SFE_UBLOX_GNSS myGPS;

// SDLogging class implementation
SDLogging::SDLogging(int csPin, int recordsPerFile) : csPin(csPin), recordsPerFile(recordsPerFile), recordCount(0), fileIndex(0) {}

void SDLogging::begin() {
    if (!SD.begin(csPin)) {
        Serial.println("SD Card initialization failed.");
    } else {
        Serial.println("SD Card initialized.");
    }
    createNewFile();
}

void SDLogging::createNewFile() {
    recordCount = 0;  // Reset record count to 0
    fileName = "log" + String(fileIndex) + ".csv";  // Set the new file name
    Serial.print("Creating new file: ");
    Serial.println(fileName);
    
    // Check if the file already exists
    if (SD.exists(fileName)) {
        File file = SD.open(fileName, FILE_READ);
        if (file.size() > 0) {
            Serial.println("File exists and is not empty, skipping header.");
            file.close();
        } else {
            file.close();
            // Create the file if it doesn't exist or is empty
            File file = SD.open(fileName, FILE_WRITE);
            if (file) {
                Serial.println("Writing header to new file.");
                file.println("Timestamp,Latitude,Longitude,Temperature,Humidity,PM1.0,PM2.5,PM4.0,PM10.0");
                file.close();
            } else {
                Serial.println("Failed to create log file.");
                return;  // Exit if the file couldn't be created
            }
        }
    } else {
        // Create the file if it doesn't exist
        File file = SD.open(fileName, FILE_WRITE);
        if (file) {
            Serial.println("Writing header to new file.");
            file.println("Timestamp,Latitude,Longitude,Temperature,Humidity,PM1.0,PM2.5,PM4.0,PM10.0");
            file.close();
        } else {
            Serial.println("Failed to create log file.");
            return;  // Exit if the file couldn't be created
        }
    }
    fileIndex++;  // Increment the file index after checking or creating the file
}


void SDLogging::logData(String data) {
    if (recordCount >= recordsPerFile) { 
        // If the current file has reached the maximum number of records, create a new file.
        Serial.println("Record count reached limit. Creating a new file...");
        createNewFile();  // Reset the recordCount and create a new file
    }
    File file = SD.open(fileName, FILE_WRITE);  // Open the file for writing
    if (file) {
        file.println(data);  // Log the data to the file
        file.close();
        recordCount++;  // Increment recordCount
        Serial.print("Data logged: ");
        Serial.println(data);
        Serial.print("Current record count: ");
        Serial.println(recordCount);
    } else {
        Serial.println("Failed to open log file for writing.");  // Error handling if file can't be opened
    }
}

// Octopus class implementation
bool Octopus::initializeSensors() {
    return HS300x.begin();
}

void Octopus::setInterval(long interval) {
    // This function can be used to set the interval for data collection if needed
}

bool Octopus::start() {
    return initializeSensors() && initializeSPS30();
}

float Octopus::readTemperature() {
    return HS300x.readTemperature();
}

float Octopus::readHumidity() {
    return HS300x.readHumidity();
}

bool Octopus::initializeSPS30() {
    sensirion_i2c_init();
    if (sps30_probe() != 0) {
        return false;
    }

    uint8_t auto_clean_days = 4;
    if (sps30_set_fan_auto_cleaning_interval_days(auto_clean_days) != 0) {
        return false;
    }

    if (sps30_start_measurement() != 0) {
        return false;
    }

    return true;
}

bool Octopus::stopSPS30() {
    return (sps30_stop_measurement() == 0);
}

bool Octopus::readSPS30Data(float &pm1_0, float &pm2_5, float &pm4_0, float &pm10_0) {
    struct sps30_measurement m;
    uint16_t data_ready;
    int16_t ret;

    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
        Serial.print("Error reading data-ready flag: ");
        Serial.println(ret);
        return false;
    } else if (!data_ready) {
        Serial.println("Data not ready, no new measurement available");
        return false;
    }

    ret = sps30_read_measurement(&m);
    if (ret < 0) {
        Serial.println("Error reading measurement");
        return false;
    } else {
        pm1_0 = m.mc_1p0;
        pm2_5 = m.mc_2p5;
        pm4_0 = m.mc_4p0;
        pm10_0 = m.mc_10p0;
        return true;
    }
}

String Octopus::getGPSTime() {
    if (myGPS.getTimeValid()) {  // Check if GPS time is valid
        String gpsTime = String(myGPS.getYear()) + "-" + 
                         String(myGPS.getMonth()) + "-" + 
                         String(myGPS.getDay()) + " " + 
                         String(myGPS.getHour()) + ":" + 
                         String(myGPS.getMinute()) + ":" + 
                         String(myGPS.getSecond());
        return gpsTime;
    } else {
        return "0.00";
    }
}

bool Octopus::initializeGPS() {
    Wire.begin();
    if (!myGPS.begin()) {
        Serial.println("Failed to initialize GPS!");
        return false;
    }

    // Set the I2C port to output UBX only (turn off NMEA noise)
    myGPS.setI2COutput(COM_TYPE_UBX);

    Serial.println("GPS initialized.");
    return true;
}

bool Octopus::readGPSData(float &latitude, float &longitude, float &altitude) {
    latitude = myGPS.getLatitude() / 10000000.0; // Convert latitude to degrees
    longitude = myGPS.getLongitude() / 10000000.0; // Convert longitude to degrees
    altitude = myGPS.getAltitude() / 1000.0; // Convert altitude to meters

    return true;
}

// Battery and LED control implementation
Adafruit_DotStar strip(1, 0, 3, DOTSTAR_BGR); // Adjust pin numbers as needed

void initBatteryMonitoring() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

float calculateBatteryPercentage(float voltage) {
    float percentage;
    if (voltage >= 4.2) {
        percentage = 100.0;
    } else if (voltage >= 3.7) {
        percentage = 100.0 - ((4.2 - voltage) / 0.5) * 20.0; // 4.2V to 3.7V range
    } else if (voltage >= 3.0) {
        percentage = 20.0 - ((3.7 - voltage) / 0.7) * 20.0; // 3.7V to 3.0V range
    } else {
        percentage = 0.0; // Below 3.0V
    }
    return percentage;
}

void setDotStarColor(uint8_t r, uint8_t g, uint8_t b) {
    float brightness = 0.1; // Adjust brightness as needed
    strip.setPixelColor(0, strip.Color(r * brightness, g * brightness, b * brightness));
    strip.show();
}

// Create an instance of SDLogging
SDLogging sd(4, 100); // Change to your CS pin and set the default records per file

// Functions to initialize and use SD functionalities
void initSD(int recordsPerFile) {
    sd = SDLogging(4, recordsPerFile); // Reinitialize the sd object with the new recordsPerFile value
    sd.begin();
}

void logToSD(String data) {
    sd.logData(data);
}
