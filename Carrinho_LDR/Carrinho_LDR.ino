#include <Servo.h>

Servo servoEsq;
Servo servoDir;

int LDR_E = A0;
int LDR_F = A1;
int LDR_D = A2;

int tolerancia = 50; // evita tremedeira

void setup() {
  servoEsq.attach(9);
  servoDir.attach(10);
  Serial.begin(9600);
}

void loop() {
  int valorE = analogRead(LDR_E);
  int valorD = analogRead(LDR_D);
  int valorF = analogRead(LDR_F);

  Serial.print("E: "); Serial.print(valorE);
  Serial.print(" F: "); Serial.print(valorF);
  Serial.print(" D: "); Serial.println(valorD);

  int velocidade = map(valorF, 0, 1023, 0, 30); 
  // ajuste fino da velocidade

  // PARADO (base)
  int base = 90;

  if (abs(valorE - valorD) < tolerancia) {
    // ANDA RETO
    servoEsq.write(base - velocidade);
    servoDir.write(base + velocidade);
  } 
  else if (valorE > valorD) {
    // MAIS LUZ NA ESQUERDA → gira esquerda
    servoEsq.write(base + velocidade);
    servoDir.write(base + velocidade);
  } 
  else {
    // MAIS LUZ NA DIREITA → gira direita
    servoEsq.write(base - velocidade);
    servoDir.write(base - velocidade);
  }

  delay(100);
}