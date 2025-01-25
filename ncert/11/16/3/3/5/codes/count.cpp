#include <Arduino.h>
int iter = 0, count = 0;
void setup() {
  Serial.begin(115200); // Start the serial communication
}

void loop() {
  iter++;
  int die = (esp_random() % 6) + 1;
  double prob = 0;
  if(die == 6){
    count++;
    prob = (double) count/iter;
    Serial.print(iter);
    Serial.print(" ");
    Serial.print(prob);
    Serial.print("\n");
  }
  if(iter > 22408) return;
  delay(10); // Wait 1 second between rolls
}

