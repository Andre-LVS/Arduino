#include <Servo.h>


#define pinServoBracoDireito 6
#define pinServoBracoEsquerdo 5

#define pinServoRodaDireita 9
#define pinServoRodaEsquerda 10

#define potServoDireito A0
#define potServoEsquerdo A1

Servo servoBracoDireito;
Servo servoBracoEsquerdo;

Servo servoRodaDireita;
Servo servoRodaEsquerda;

//-----------------Protótipos---------------
void modoBracoRobo();
void modoCarrinho();

//--------------------------------------------
//Está função  e variáveis servem para eu usar uma mesma interrupção
//mas cada botão diferente faz uma função diferente 
void interrupcoBotoaoBranco();
void interrupcoBotoaoVermelho();
void acoesDosBotoes();

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

const int botaoAzulEsquerdo = 13; //Botão para o robô. Um lado do botão no pino 13 e outro no GND.
const int botaoAzulDireito = 12;
const int botaoVermelho = 2; //*Somente pino 2 e 3 tem interrupção
const int botaoBranco = 3;

//---------------

//variáveis globais

int anguloServoBracoDireito;
int anguloServoBracoEsquerdo;
volatile byte modo = 0; //modo = 0 -> Braço robo;
                       // modo = 1 -> Modo carrinho

//variável para quardar os valores dos ângulos dos servos das rodas
int motor1, motor2;
volatile byte botaoPressionado = 0;
volatile bool pausado = false;
int tempoDelay = 100;

void setup() {
  // put your setup code here, to run once:]
  Serial.begin(9600); //começar comunicação serial
  Serial.print("Inicio:");
  //------------BOTÕES-----------------
  pinMode(botaoAzulEsquerdo, INPUT_PULLUP);
   pinMode(botaoAzulDireito, INPUT_PULLUP);    //IPUT_PULLUP deixa em estado 
   pinMode(botaoVermelho, INPUT_PULLUP);//lógico alto e detecta quando vai para zero
   pinMode(botaoBranco,INPUT_PULLUP);
  //-----------------------------------

  //----------SERVOS-------------------
  //atribuir pinos dos servos
  servoBracoDireito.attach(pinServoBracoDireito);
  servoBracoEsquerdo.attach(pinServoBracoEsquerdo);

  servoRodaDireita.attach(pinServoRodaDireita);
  servoRodaEsquerda.attach(pinServoRodaEsquerda);

  //posição inicial servos:
  servoBracoDireito.write(0);
  servoBracoEsquerdo.write(180);
  //-----------------------------------

 //---------INTERRUPÇÕES--------------
  attachInterrupt(
    digitalPinToInterrupt(botaoBranco),
    interrupcoBotoaoBranco, //função a ser chamada na interrupção
    FALLING //borda de descida
  );

  attachInterrupt(
    digitalPinToInterrupt(botaoVermelho),
    interrupcoBotoaoVermelho, //função a ser chamada na interrupção
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

  //----------------------------------
}

void loop() {
  // put your main code here, to run repeatedly:
  if (pausado) { //Se o botão vermelho for apertado, pausa o programa
        servoRodaDireita.write(90);
        servoRodaEsquerda.write(90);

        while (pausado) {
          Serial.println("PAUSADO");
            delay(50);
        }
  
    
    
  }

  
  modoBracoRobo();
  modoCarrinho();

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
  Serial.println(anguloServoBracoDireito);

  Serial.print("Angulo Esquerdo:");
  Serial.println(anguloServoBracoEsquerdo);

  if(modo == 0){
   Serial.print("Modo robô:");
  Serial.println(modo);
 }
 else if (modo ==1){
   Serial.print("Modo Carrinho ângular:");
  Serial.println(modo);
 }

  else if (modo ==2){
   Serial.print("Modo Carrinho linear:");
  Serial.println(modo);
 }
  

//==========================================
    mostradorTimer = millis();
  }

}
//--------------------------------------------

//-------------------------------------------
void interrupcoBotoaoVermelho(){

pausado = !pausado;

}
//-------------------------------------------

void interrupcoBotoaoBranco(){
  
 if(modo == 0){
  modo = 1;
  return;
 }
 else if (modo ==1){
  modo = 2;
  return;
 }

  else if (modo ==2){
  modo = 0;
  return;
 }
 

}
//--------------------------------------------

void acoesDosBotoes(){

  if(digitalRead(botaoAzulEsquerdo) == LOW && digitalRead(botaoAzulDireito) == LOW) {
  
      botaoPressionado = 1;

  }
  

  else if(digitalRead(botaoAzulDireito) == LOW){
  
      botaoPressionado = 2;

  }

  else if(digitalRead(botaoAzulEsquerdo) == LOW){
  
      botaoPressionado = 3;

  }




  switch(botaoPressionado){

    case 1:
      //ação de quando ambos os botões azulis forem apertados no modo Braço Robo
      Serial.println("Dois botões Azuis apertados:");
      botaoPressionado = 0;
      delay(tempoDelay);
      break;

    case 2: 
      //ação do botão Azul Direito quando estiver no modo Braço Robo
      Serial.println("Botão Azul Direito apertado:");
      botaoPressionado = 0;
      delay(tempoDelay);
      break;

    case 3:
      //Ação do botão Azul Esquerdo quando estiver no modo Braço Robo
      Serial.println("Botão Azul Esquerdo apertado:");
      botaoPressionado = 0;
      delay(tempoDelay);
      break;

    default:
        break;

  }

}
//--------------------------------------------

void modoBracoRobo(){
  
if(modo == 0){
  acoesDosBotoes();
  int leituraAnalogicaPotDireito= map(analogRead(potServoDireito), 0, 1023, 0, 255); //mapear a entrada analogica(10 bits) com a saida analogica (8bits)
  int leituraAnalogicaPotEsquerdo= map(analogRead(potServoEsquerdo), 0, 1023, 0, 255);
  
  anguloServoBracoDireito = map(leituraAnalogicaPotDireito, 0, 255, 0, 95); //mapear a saída analogica com o angulo
  anguloServoBracoEsquerdo = map(leituraAnalogicaPotEsquerdo, 255, 0, 0, 180);
  delay(50);
  servoBracoDireito.write(anguloServoBracoDireito); //repouso 0
  servoBracoEsquerdo.write(anguloServoBracoEsquerdo); //repouso 180
}
 
}

void modoCarrinho(){
 int PARADO = 90;
 //Movimento Angular
 if(modo == 1){
     // PARADO (base)
 


   while(digitalRead(botaoAzulEsquerdo) == LOW){

        if (pausado) {
        servoRodaDireita.write(90);
        servoRodaEsquerda.write(90);
        return;
        }
  
    motor1 = 160;
    motor2 = 160;
    servoRodaDireita.write(motor1);
    servoRodaEsquerda.write(motor2);
    Serial.println("Botão Azul 1 apertado:");
    

  }

  

 
   while(digitalRead(botaoAzulDireito) == LOW){

        if (pausado) {
        servoRodaDireita.write(90);
        servoRodaEsquerda.write(90);
        return;
        }
  
    motor1 = 20;
    motor2 = 20;
    servoRodaDireita.write(motor1);
    servoRodaEsquerda.write(motor2);
    Serial.println("Botão Azul 2 apertado:");
    

  }
  servoRodaDireita.write(PARADO);
  servoRodaEsquerda.write(PARADO);
  
  delay(200);
  
 }

//Movimento Linear
  if(modo == 2){
     // PARADO (base)
  


   while(digitalRead(botaoAzulEsquerdo) == LOW){

        if (pausado) {
        servoRodaDireita.write(90);
        servoRodaEsquerda.write(90);
        return;
        }
  
    motor1 = 20;
    motor2 = 160;
    servoRodaDireita.write(motor1);
    servoRodaEsquerda.write(motor2);
    Serial.println("Botão Azul 1 apertado:");
    

  }

  

 
   while(digitalRead(botaoAzulDireito) == LOW){

        if (pausado) {
        servoRodaDireita.write(90);
        servoRodaEsquerda.write(90);
        return;
        }
  
    motor1 = 160;
    motor2 = 20;
    servoRodaDireita.write(motor1);
    servoRodaEsquerda.write(motor2);
    Serial.println("Botão Azul 2 apertado:");
    

  }
  servoRodaDireita.write(PARADO);
  servoRodaEsquerda.write(PARADO);
  
  delay(200);
  
 }

}

