#include <Servo.h>

//Pinos dos servos
#define pinServo1 3
#define pinServo2 5

//Pinos potenciômetro
#define pot1 A0
Servo serv1;
Servo serv2;

//variável para quardar os valores dos ângulos
int motor1, motor2;

//----------------------------------
const int botao = 2; //Botão para parar o robô. Um lado do botão no pino 2 e outro no GND.
volatile bool pausado = false;

//-------------------------------------

unsigned long mostradorTimer = 1;
const unsigned long intervaloMostrador = 2000;

int stopSignal = 90;   // ajustar na calibração
int deadZone = 20;     // zona morta para evitar tremor

  int estado = 0;
  int contador =0;

void setup() {
  // put your setup code here, to run once:

  //-----------Interrupção no botão-----------------
  // o botão é detectado na hora
  //mas o código só “para de verdade” quando sair do delay()
  pinMode(botao, INPUT_PULLUP);

    
  attachInterrupt(
    digitalPinToInterrupt(botao),
    alternarPausa,
    FALLING
  );
  //-----------------------------

  Serial.begin(9600); //começar comunicação serial

//atribuir pinos dos servos
  serv1.attach(pinServo1);
  serv2.attach(pinServo2);


}

void loop() {
  // put your main code here, to run repeatedly:
  
  //leitura do potenciômetro
  //motor1 = map(analogRead(pot1),20,1010,0,360);

/*
if(motor1 >(stopSignal - deadZone) && motor1<(stopSignal + deadZone)){
serv1.write(stopSignal); // para
  } else {
    serv1.write(motor1); // gira com velocidade proporcional
  }
  */

//-----------Lógica para para o robo---------
  // Enquanto estiver pausado
  while (pausado) {
    Serial.println("PAUSADO");
    delay(200);
  }

//-------------------------------------------


  while(estado==0){

    motor1 = 140;
    motor2 = 40;
   serv1.write(motor1);
    serv2.write(motor2);
    contador++;
    delay(20);

    if(contador == 50){
      estado = 1;
      contador = 0;
    }

  }

    while(estado==1){

    motor1 = 40;
    motor2 = 140;
 serv1.write(motor1);
 serv2.write(motor2);
    contador++;
    delay(20);

    if(contador == 50){
      estado = 2;
      contador = 0;
    }

   }

   while(estado==2){

    motor1 = 90;
    motor2 = 90;
 serv1.write(motor1);
 serv2.write(motor2);
    contador++;
    delay(20);

    if(contador == 50){
      estado = 0;
      contador = 0;
    }

   }

  
delay(20);







  if ((millis() - mostradorTimer) >= intervaloMostrador) {

  // envio para o monitor serial do posicionamentos dos motores
  Serial.println("****************************");
  
  //Serial.print("Pot1:");
  //Serial.print(analogRead(pot1));

    Serial.print("Estado:");
  Serial.print(estado);

     Serial.print("contador:");
  Serial.print(contador);

  Serial.print(" Angulo Motor1:");
  Serial.println(motor1);

    mostradorTimer = millis();
  }

   // tempo de espera para recomeçar
   //  Serial.println("Pot1:");
  //Serial.println(analogRead(motor1));
  delay(200);
}

void alternarPausa() {
  pausado = !pausado;
}
