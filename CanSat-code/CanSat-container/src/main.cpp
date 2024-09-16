#include <Arduino.h>
#include <SD.h>
#include "../lib/MPL3115A2.hpp"
#include "../lib/MPU6050.hpp"
#include "../lib/PA1616S.hpp"
#include "../lib/LSM303AGR.hpp"
#include "../lib/XBEE.hpp"
#include "../include/mission-control-handler.hpp"
#include "../include/rules-engine.hpp"
#include "../include/mode-select.hpp"
#include "../include/container-dto.hpp"
#include "../include/camera-system.hpp"

using namespace CanSat;

ModeSelect mode_select;
MissionControlHandler mission_control_handler;
Container_Data container_data;

XBEE Xbee(14, 15);
MPL3115A2 Barometer(17, 16);
MPU6050 IMU(18, 19);
LSM303AGR Compass(25, 24);
CameraSystem camera_system(41, 27, 26, 21, 20);
PA1616S GPS;
RulesEngine rules_engine;
String json_data = "";
int heartbeat = 0;
File dataFile;
int sd_card_counter = 10; // Number of times to check for a SD card until giving up
bool sd_card_active = true;
/**
 * @brief Updates each sensor, and maps updated data to the container_data object
 * 
 * @param container_data current data transfer object
 * @return Container_Data updated data transfer object
 */
Container_Data ReadAllSensors(Container_Data container_data){
    // Update all sensors
    Barometer.Update();
    IMU.Update();
    GPS.Update();
    Compass.Update();

    container_data.heartbeat_count = ++heartbeat;

    container_data.imu_data.acceleration_x = IMU.GetAccelerometer().x;
    container_data.imu_data.acceleration_y = IMU.GetAccelerometer().y;
    container_data.imu_data.acceleration_z = IMU.GetAccelerometer().z;
    container_data.imu_data.gyro_x = IMU.GetGyroscope().x;
    container_data.imu_data.gyro_y = IMU.GetGyroscope().y;
    container_data.imu_data.gyro_z = IMU.GetGyroscope().z;
    container_data.imu_data.pitch = IMU.GetAttitude().pitch;
    container_data.imu_data.roll = IMU.GetAttitude().roll;
    
    container_data.gps_data.latitude = GPS.GetLatitude();
    container_data.gps_data.longitude = GPS.GetLongitude();
    container_data.gps_data.exactAltitude = GPS.GetAltitude();

    container_data.barometer_data.temperature = Barometer.GetData().temperature;
    container_data.barometer_data.relativeAltitude = Barometer.GetData().relativeAltitude;

    container_data.compass_data.magnet_x = Compass.GetMagneticField().x;
    container_data.compass_data.magnet_y = Compass.GetMagneticField().y;
    container_data.compass_data.magnet_z = Compass.GetMagneticField().z;


    return container_data;
}


void saveToSD(String data) {
    if (sd_card_active == false) return;
    dataFile = SD.open("flight_data.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.println(data);   
        dataFile.close();
    }
    else {
        Serial.println("Error opening flight data file");
    }
}

void clearSDCard() {
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SD card initialization failed!");
        sd_card_active = false;
        return;
    }
    
    dataFile = SD.open("flight_data.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.truncate(0);
        dataFile.close();
    }
    else {
        Serial.println("Error opening flight data file");
    }
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    Barometer.Initialize(10.0);
    IMU.Initialize();
    GPS.Initialize();
    Compass.Initialize();
    Xbee.Initialize();
    clearSDCard();

    container_data.id = 'C';
    container_data.flight_mode = 'U';
}


void loop() {
    container_data = ReadAllSensors(container_data);
    container_data = rules_engine.MainValidation(container_data);
    container_data = mode_select.SelectMode(container_data);

    camera_system.TakeVideoAll(5000);
    
    json_data = mission_control_handler.FormatContainerData(container_data);
    Serial.println(json_data);
    Xbee.transmitData(json_data);
    saveToSD(json_data);

    delay(50);
}