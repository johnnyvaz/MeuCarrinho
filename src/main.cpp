#include <Arduino.h>
#include <Wire.h>
#include <AFMotor.h>    //Biblioteca do motor shield
#include <Ultrasonic.h> //Bibloteca do sensor ultrassônico
#include <LiquidCrystal_I2C.h>
#include <PID_v2.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);

//********************Definição dos pinos********************

#define trigger A2
#define echo A3

// #define TRIGGER   2 //pino de trigger do sensor
// #define ECHO      3 //pino de echo do sensor
#define VEL_SOMu  0.000340 //velocidade do som para calculos
#define M1F  5 //pino de tensão direta no motor 1
#define M2F  6 //pino de tensão direta no motor 2
#define M1R  9 //pino de tensão reversa no motor 1
#define M2R  10 //pino de tensão reversa no motor 2
#define TAM_FILTRO  10 //tamanho do vetor de filtro

//********************Criação dos objetos********************

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Ultrasonic ultrasonic(trigger, echo);

void parada();
void tras();
void esquerda();
void frente();

//********************Função de configuração******************

void setup()
{
  // Configuração de velocidade dos motores
  motor1.setSpeed(100); // Velocidade vai de 0 a 255
  motor2.setSpeed(100);
  motor3.setSpeed(100);
  motor4.setSpeed(100);
  Serial.begin(9600); // Habilita a comunicação serial

  lcd.init(); // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" * JOHNNY VAZ * ");
  lcd.setCursor(0, 1);
  lcd.print("     LEOBOT ");
}

//********************Função de principal********************

void loop()
{

  // Cálculo e conversão da leitura do sensor ultrassônico
  long microsec = ultrasonic.timing();
  float distancia = ultrasonic.convert(microsec, Ultrasonic::CM);
  // Mostra as leitura dos sensores
  // Quando for ler remova o /* */

  // Serial.print("A distancia: ");
  // Serial.println(distancia);
  // delay(1000);

  // Verifica se tem alguma coisa na sua frente e se os sensor estão no branco
  if (distancia < 20)
  {
    lcd.setCursor(0, 0);
    lcd.print("< 20 : TRAS ");
    parada();
    tras();
    lcd.setCursor(0, 1);
    lcd.print(distancia);
    delay(500);
    esquerda();
    delay(500);
  }
  // Se caso encontrar algo na sua frente
  if (distancia >= 20)
  {
    // while (digitalRead(sensor1) == 0) { //Prende na condição até o sensor encotrar a borda da arena
    Serial.println("frente");
    lcd.setCursor(0, 0);
    lcd.print(">= 20 : FRENTE");
    frente(); // Movimenta para frente
    lcd.setCursor(0, 1);
    lcd.print(distancia);
  }
}
//********************Movimenta o robô para frente********************

void frente()
{
  Serial.println("frente");
  motor1.run(FORWARD); // Motor traseiro esquerdo
  motor2.run(FORWARD); // Motor dianteiro esquerdo
  motor3.run(FORWARD); // Motor dianteiro direito
  motor4.run(FORWARD); // Motor traseiro direito
}
//********************Movimenta o robô para trás********************

void tras()
{
  Serial.println("tras");
  motor1.run(BACKWARD); // Motor traseiro esquerdo
  motor2.run(BACKWARD); // Motor dianteiro esquerdo
  motor3.run(BACKWARD); // Motor dianteiro direito
  motor4.run(BACKWARD); // Motor traseiro direito
}

//********************Movimenta o robô para esquerda********************

void esquerda()
{
  Serial.println("esquerda");
  motor1.run(FORWARD);  // Motor traseiro esquerdo
  motor2.run(FORWARD);  // Motor dianteiro esquerdo
  motor3.run(BACKWARD); // Motor dianteiro direito
  motor4.run(BACKWARD); // Motor traseiro direito
}

void parada()
{
  Serial.println("parada");
  motor1.run(RELEASE); // Motor traseiro esquerdo
  motor2.run(RELEASE); // Motor dianteiro esquerdo
  motor3.run(RELEASE); // Motor dianteiro direito
  motor4.run(RELEASE); // Motor traseiro direito
}