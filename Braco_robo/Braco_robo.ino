#include <Servo.h>


#define pinServoDireito 6
#define pinServoEsquerdo 5

#define potServoDireito A0
#define potServoEsquerdo A1

Servo servoDireito;
Servo servoEsquerdo;

void bracoRobo();
//--------------------------------------------
//Está função  e variáveis servem para eu usar uma mesma interrupção
//mas cada botão diferente faz uma função diferente 
void interrupcoBotoes();
void acoesDosBotoes();
volatile byte botaoPressionado = 0;
//-------------------------------------------

//--------------------------------------------
//Está função  e variáveis servem para o terminal 
//não ficar spammando informções numa velocidade absurda.
void ImprimeSerialEmIntervalos();
unsigned long mostradorTimer = 1;
const unsigned long intervaloMostrador = 2000;
/*Se intervaloMostrador for:
1000 → imprime 1 vez por segundo.
500 → imprime 2 vezes por segundo.
100 → imprime 10 vezes por segundo.*/
//--------------------------------------------

const int botaoAzul = 13; //Botão para o robô. Um lado do botão no pino 13 e outro no GND.
const int botaoVermelho = 12;
const int botaoBranco = 11;

//-------deletarr---
int brightness = 0;  // how bright the LED is
int fadeAmount = 25;  // how many points to fade the LED by
//---------------

//variáveis globais

int anguloServoDireito;
int anguloServoEsquerdo;

void setup() {
  // put your setup code here, to run once:]
  Serial.begin(9600); //começar comunicação serial
  Serial.print("Inicio:");
  //------------BOTÕES-----------------
   pinMode(botaoAzul, INPUT_PULLUP);    //IPUT_PULLUP deixa em estdo 
   pinMode(botaoVermelho, INPUT_PULLUP);//lógico alto e detecta quando vai para zero
   pinMode(botaoBranco,INPUT_PULLUP);
  //-----------------------------------

  //----------SERVOS-------------------
  //atribuir pinos dos servos
  servoDireito.attach(pinServoDireito);
  servoEsquerdo.attach(pinServoEsquerdo);

  //posição inicial servos:
  servoDireito.write(0);
  servoEsquerdo.write(180);
  //-----------------------------------

 /* //---------INTERRUPÇÃO--------------
  attachInterrupt(
    digitalPinToInterrupt(botaoBranco),
    interrupcoBotoes, //função a ser chamada na interrupção
    FALLING //borda de descida
  );
/*Modo	Dispara quando
RISING	O sinal passa de LOW → HIGH (borda de subida)
FALLING	O sinal passa de HIGH → LOW (borda de descida)
CHANGE	Em qualquer mudança (LOW ↔ HIGH)
LOW	Enquanto o pino permanecer em nível baixo

RISING:
LOW  ____|‾‾‾‾
          ↑

FALLING:
HIGH ‾‾‾‾|____
          ↑

CHANGE:
____|‾‾‾‾|____
    ↑    ↑

*/

  //----------------------------------*/
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print("loop:");
  acoesDosBotoes();
  bracoRobo();

  ImprimeSerialEmIntervalos();
}
//-------------fim do loop ------------------






//---------------funções----------------

void ImprimeSerialEmIntervalos(){
//Está função serve como um limitador de frequência para as mensagens do Serial Monitor.
//Para o terminal não ficar spammando informções numa velocidade absurda.
//A cada alguns segundo o terminal atualiza.
//colocar entre os //== as informações para serem impressas.
if ((millis() - mostradorTimer) >= intervaloMostrador) {
//===========================================

  Serial.println(F("****************************"));
  
  Serial.print("Angulo Direito:");
  Serial.println(anguloServoDireito);

  Serial.print("Angulo Esquerdo:");
  Serial.println(anguloServoEsquerdo);

  

//==========================================
    mostradorTimer = millis();
  }

}
//--------------------------------------------

void interrupcoBotoes(){
  
 

}
//--------------------------------------------

void acoesDosBotoes(){

   if(digitalRead(botaoAzul) == LOW){
  
      botaoPressionado = 1;

  }

  else if(digitalRead(botaoVermelho) == LOW){
  
      botaoPressionado = 2;

  }

  else if(digitalRead(botaoBranco) == LOW){
  
      botaoPressionado = 3;

  }

  switch(botaoPressionado){

    case 1:
      //ação do botão Azul
      Serial.print("Botão Azul apertado:");
      botaoPressionado = 0;
      break;

    case 2: 
      //ação do botão Vermelho
      Serial.print("Botão Vermelho apertado:");
      botaoPressionado = 0;
      break;

    case 3:
      //Ação do botão Branco
      Serial.print("Botão Branco apertado:");
      botaoPressionado = 0;
      break;

    default:
        break;

  }

}
//--------------------------------------------

void bracoRobo(){

  int leituraAnalogicaPotDireito= map(analogRead(potServoDireito), 0, 1023, 0, 255);
  int leituraAnalogicaPotEsquerdo= map(analogRead(potServoEsquerdo), 0, 1023, 0, 255);
  
  anguloServoDireito = map(leituraAnalogicaPotDireito, 0, 255, 0, 95);
  anguloServoEsquerdo = map(leituraAnalogicaPotEsquerdo, 255, 0, 0, 180);
  delay(250);
 servoDireito.write(anguloServoDireito); //repouso 0
  servoEsquerdo.write(anguloServoEsquerdo); //repouso 180
  
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 65) {
    fadeAmount = -fadeAmount;}
  // servoDireito.write(0);
    // delay(100);
 //servoEsquerdo.write(180);
  
/*for(int i =0; i<=10;i++){
   delay(1000);
   servoDireito.write(0);
   delay(1000);
    servoEsquerdo.write(180);
    delay(1000);
    servoDireito.write(45);
     delay(1000);
    servoEsquerdo.write(125);
    }
*/
   

}