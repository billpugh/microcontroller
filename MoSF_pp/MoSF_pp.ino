#include <Adafruit_NeoPixel.h>

const uint8_t engine_pin = 1;
const uint8_t computer_pin = 3;
const uint8_t particleCollector_pin = 5;
const uint8_t anomalyCollector_pin = 7;

Adafruit_NeoPixel fuelTanks = Adafruit_NeoPixel(18, 22, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel warpCore = Adafruit_NeoPixel(8, 20, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel onBoardComputer = Adafruit_NeoPixel(8, 18, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel particleCollector = Adafruit_NeoPixel(16, 16, NEO_GRB + NEO_KHZ800);

uint8_t engineOn, computerOn, particleCollectorOn, anomalyCollectorOn;
unsigned long engineChange, computerChange, particleCollectorChange, anomalyCollectorChange;

void setup() {
  Serial.begin(115200);
  pinMode(engine_pin, INPUT);
  pinMode(computer_pin, INPUT);
  pinMode(particleCollector_pin, INPUT);
  pinMode(anomalyCollector_pin, INPUT);

  delay(2000);

  Serial.println("MoSF proton probe active");
  if (digitalRead(engine_pin))
    Serial.println("Engine on");
  if (digitalRead(computer_pin))
    Serial.println("Computer on");
  if (digitalRead(particleCollector_pin))
    Serial.println("Particle Collector on");
  if (digitalRead(anomalyCollector_pin))
    Serial.println("Anomaly Collector on");


  engineOn =  computerOn = particleCollectorOn = anomalyCollectorOn = 0;
  engineChange = computerChange = particleCollectorChange = anomalyCollectorChange = 0;
  fuelTanks.begin();
  fuelTanks.show();
  warpCore.begin();
  warpCore.show();
  onBoardComputer.begin();
  onBoardComputer.show();
  particleCollector.begin();
  particleCollector.show();

}

void loop() {
  uint8_t value;
  unsigned long ms = millis();
  if (engineOn != (value = digitalRead(engine_pin))) {
    engineOn = value;
    engineChange = ms;
  }
  if (onBoardComputerOn != (value = digitalRead(onBoardComputer_pin))) {
    onBoardComputerOn = value;
    onBoardComputerChange = ms;
  }
  if (particleCollectorOn != (value = digitalRead(particleCollector_pin))) {
    particleCollectorOn = value;
    particleCollectorChange = ms;
  }
  if (anomalyCollectorOn != (value = digitalRead(anomalyCollector_pin))) {
    anomalyCollectorOn = value;
    anomalyCollectorChange = ms;
  }

}






