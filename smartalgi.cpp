

#define NUM_CELLS 6       // Change this to any number of cells
#define TOLERANCE 1.0     // Balance tolerance in %
#define BALANCE_STEP 0.5  // % SoC adjusted per cycle
#define BALANCE_PWM 120   // PWM duty cycle for balancing MOSFETs

float soc[NUM_CELLS] = {20, 45, 60, 55, 80, 30}; // Example values

int balancePins[NUM_CELLS] = {3,5,6,9,10,11}; // Adjust for your hardware

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_CELLS; i++) {
    pinMode(balancePins[i],OUTPUT);
    analogWrite(balancePins[i], 0); // start OFF
  }
  Serial.println("Universal Active Balancing BMS Initialized");
}

void loop() {
  float avg_soc = calculateAverageSOC();
  balanceAllCells(avg_soc);

  // Print SoCs
  Serial.print("SoC Levels: ");
  for (int i = 0; i < NUM_CELLS; i++) {
    Serial.print(soc[i], 1);
    Serial.print("% ");
  }
  Serial.print("| Avg: ");
  Serial.println(avg_soc, 1);

  delay(1000);
}

// ---- FUNCTIONS ----

float calculateAverageSOC() {
  float sum = 0;
  for (int i = 0; i < NUM_CELLS; i++) sum += soc[i];
  return sum / NUM_CELLS;
}

void balanceAllCells(float avg_soc) {
  for (int i = 0; i < NUM_CELLS; i++) {
    if (soc[i] > avg_soc + TOLERANCE) {
      donateEnergy(i);
    } else if (soc[i] < avg_soc - TOLERANCE) {
      receiveEnergy(i);
    } else {
      stopBalancing(i);
    }
  }
}

void donateEnergy(int cellIndex) {
  analogWrite(balancePins[cellIndex], BALANCE_PWM);
  soc[cellIndex] -= BALANCE_STEP; // simulate donation
  Serial.print("Cell "); Serial.print(cellIndex+1);
  Serial.println(" donating energy...");
}

void receiveEnergy(int cellIndex) {
  analogWrite(balancePins[cellIndex], BALANCE_PWM);
  soc[cellIndex] += BALANCE_STEP; // simulate receiving
  Serial.print("Cell "); Serial.print(cellIndex+1);
  Serial.println(" receiving energy...");
}

void stopBalancing(int cellIndex) {
  analogWrite(balancePins[cellIndex], 0);
}
