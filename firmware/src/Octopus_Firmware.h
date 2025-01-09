#ifndef OCTOPUS_H
#define OCTOPUS_H

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino_HS300x.h>
#include <Adafruit_DotStar.h>
#include <sps30.h> // Include the SPS30 library
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>

// Define the SD classes
class SDLogging {
public:
    SDLogging(int csPin, int recordsPerFile);
    void begin();
    void logData(String data);
private:
    int csPin;
    int recordsPerFile;
    int recordCount;
    int fileIndex;
    String fileName;
    void createNewFile();
};

class Octopus {
public:
    static bool initializeSensors();
    static void setInterval(long interval);
    static bool start();
    static float readTemperature();
    static float readHumidity();

    // SPS30 Functions
    static bool initializeSPS30(); // Add function for initializing SPS30
    static bool readSPS30Data(float &pm1_0, float &pm2_5, float &pm4_0, float &pm10_0); // Add function for reading SPS30 data
    static bool stopSPS30(); // Add function to stop SPS30 measurement

    // GPS Functions
    static bool initializeGPS(); // Initialize GPS
    static bool readGPSData(float &latitude, float &longitude, float &altitude); // Read GPS data
    static String getGPSTime(); // Get GPS time as a string

};

// Battery and LED control functions
void initBatteryMonitoring();
float calculateBatteryPercentage(float voltage);
void setDotStarColor(uint8_t r, uint8_t g, uint8_t b);

// Functions for SD Card Handling
void initSD(int recordsPerFile);
void logToSD(String data);

#endif // OCTOPUS_H
