#include <Arduino.h>
#include "../../../lib/MiniSpyCamera.hpp"
using namespace CanSat;

MiniSpyCamera camera(41);

void setup() {
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Starting Camera Demo!");
  camera.TakeImage();
  camera.TakeVideo(5000);
  delay(1000);
}