#include <SoftwareSerial.h>
#include <Servo.h>

// ===== BLUETOOTH =====
// RX = Pin 2 
// TX = Pin 3 
SoftwareSerial bluetooth(2, 3); 

//  L298N MOTOR PINS (Kept from Maze Code) =====
#define ENA 5
#define ENB 6   
#define IN1 10  
#define IN2 7   
#define IN3 8   
#define IN4 9   

// ===== SERVOS =====
#define SERVO_BRAS 12  // Arm Up/Down
#define SERVO_PINCE 13 // Claw Open/Close

Servo bras;
Servo pince;

// ===== SERVO POSITIONS =====
const int BRAS_HAUT = 140;
const int BRAS_BAS = 0;
const int BRAS_MILIEU = 90;

const int PINCE_OUVERTE = 40;
const int PINCE_FERMEE = 80;

// ===== SPEED CALIBRATION  =====
int SPEED_RIGHT_FWD = 140; 
int SPEED_LEFT_FWD  = 147; 

int SPEED_RIGHT_BWD = 140; 
int SPEED_LEFT_BWD  = 147; 

int TURN_SPEED = 140; // Tank turn speed for Left/Right

// ===== ARM SWEEP STATE =====
bool movingUp = false;
bool movingDown = false;
unsigned long lastArmMove = 0;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Initialize Motor Pins
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Initialize Servos
  bras.attach(SERVO_BRAS);
  pince.attach(SERVO_PINCE);

  // Set Initial Servo Positions
  bras.write(BRAS_MILIEU);
  pince.write(PINCE_OUVERTE);

  // Ensure motors are off at startup
  stopMotors();
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {
  // 1. READ BLUETOOTH COMMANDS
  if (bluetooth.available()) {
    char cmd = bluetooth.read();
    
    // Ignore invisible characters like enters and spaces
    if (cmd != '\n' && cmd != '\r' && cmd != ' ') {
      
      switch (cmd) {
        // --- WHEEL CONTROL ---
        case 'F': forward(); break;
        case 'B': backward(); break;
        case 'L': left(); break;
        case 'R': right(); break;
        case 'S': // Stop command from App
          stopMotors(); 
          movingUp = false; 
          movingDown = false; 
          break;

        // --- CLAW CONTROL ---
        case 'O': pince.write(PINCE_OUVERTE); break;
        case 'C': pince.write(PINCE_FERMEE); break;

        // --- ARM CONTROL (Continuous Sweep) ---
        case 'U': 
          movingUp = true; 
          movingDown = false; 
          break;
        case 'D': 
          movingDown = true; 
          movingUp = false; 
          break;
        case 'X': // Optional: Instant center arm
          bras.write(BRAS_MILIEU); 
          movingUp = false; 
          movingDown = false;
          break;
      }
    }
  }

  // 2. CONTINUOUS ARM SWEEP LOGIC
  // This allows the arm to move smoothly while holding the button
  if (millis() - lastArmMove > 20) { 
    if (movingUp) {
      bras.write(constrain(bras.read() + 1, BRAS_BAS, BRAS_HAUT));
    }
    if (movingDown) {
      bras.write(constrain(bras.read() - 1, BRAS_BAS, BRAS_HAUT));
    }
    lastArmMove = millis();
  }
}

// =====================================================
// MOVEMENT FUNCTIONS
// =====================================================

void forward() {
  analogWrite(ENA, SPEED_RIGHT_FWD);
  analogWrite(ENB, SPEED_LEFT_FWD);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); // Right Forward
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // Left Forward
}

void backward() {
  analogWrite(ENA, SPEED_RIGHT_BWD);
  analogWrite(ENB, SPEED_LEFT_BWD);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Right Backward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  // Left Backward
}

void left() {
  // Tank Spin Left: Right wheel forwards, Left wheel backwards
  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH); // Right Forward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);  // Left Backward
}

void right() {
  // Tank Spin Right: Right wheel backwards, Left wheel forwards
  analogWrite(ENA, TURN_SPEED);
  analogWrite(ENB, TURN_SPEED);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Right Backward
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // Left Forward
}

void stopMotors() {
  analogWrite(ENA, 0); 
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
