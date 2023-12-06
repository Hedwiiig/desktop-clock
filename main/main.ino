#include <SoftwareSerial.h>

// Configurações do motor de passo =================================
#define passos 8  // Número de passos por revolução do motor
int port[4] = {11, 10, 9, 8};  // Pinos usados para controlar o motor
int delaytime = 1;
static int phase = 0;
int i, j;

int seq[8][4] = {
  {  LOW, HIGH, HIGH,  LOW},// 8 estados diferentes pois o motor e de 4 fases 4*2
  {  LOW,  LOW, HIGH,  LOW},
  {  LOW,  LOW, HIGH, HIGH},
  {  LOW,  LOW,  LOW, HIGH},
  { HIGH,  LOW,  LOW, HIGH},
  { HIGH,  LOW,  LOW,  LOW},
  { HIGH, HIGH,  LOW,  LOW},
  {  LOW, HIGH,  LOW,  LOW}
};
//função de rotação=================================================

void rotacao(int step) {

  for (j = 0; j < step; j++) {
    phase = (phase + 1) % 8; //se resto = 0 entao restart.
    for (i = 0; i < 4; i++) {
      digitalWrite(port[i], seq[phase][i]);
    }
    delay(delaytime);
  }
  // power cut
  for (i = 0; i < 4; i++) { // todos a low
    digitalWrite(port[i], LOW);
  }
}

// Configurações do Bluetooth ======================================
#define rx 0
#define tx 1

unsigned int h = 0, m = 0, s = 0;
unsigned int total = 0;
SoftwareSerial bluetooth(rx, tx); // RX, TX

// Configurações do buzzer =========================================
unsigned int count;
#define speaker 6

// Configurações do Sensor Ultrassônico HC-SR04 =====================
# define trigPin 13
# define echoPin 12
int distancia = 10;

// Função para medir a distância com o sensor ultrassônico
float measureDistance() {
  float duration, distance;

  // Envia pulso
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Aguarda o eco e mede o tempo até que ocorra
  duration = pulseIn(echoPin, HIGH);

  // Calcula a distância
  distance = (duration * 0.0343) / 2;

  return distance;
}

void setup() {
  // Configuração dos pinos como saída
  for (i = 0; i < 4; i++) {
    pinMode(port[i], OUTPUT);
  }
  pinMode(speaker, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Inicializa a comunicação serial
  Serial.begin(9600);
}

void loop() {

  rotacao(passos);

  // power cut
  for (i = 0; i < 4; i++) { // todos a low
    digitalWrite(port[i], LOW);
  }
  // Verifica se há dados disponíveis no módulo Bluetooth
  if (bluetooth.available() > 0) {
    // Lê valor de horas
    bluetooth.print("horas= ");
    h = bluetooth.parseInt(); // Lê o valor de horas
    while (bluetooth.read() != '\n') {} // Aguarda a quebra de linha após o valor de horas
    bluetooth.println(""); // Envia uma quebra de linha

    // Lê valor de minutos
    bluetooth.print("minutos= ");
    m = bluetooth.parseInt(); // Lê o valor de minutos
    while (bluetooth.read() != '\n') {} // Aguarda a quebra de linha após o valor de minutos
    bluetooth.println(""); // Envia uma quebra de linha

    // Lê valor de segundos
    bluetooth.print("segundos= ");
    s = bluetooth.parseInt(); // Lê o valor de segundos
    while (bluetooth.read() != '\n') {} // Aguarda a quebra de linha após o valor de segundos
    bluetooth.println(""); // Envia uma quebra de linha

    // Exibe os valores lidos no Monitor Serial
    Serial.print("horas= ");
    Serial.print(h);
    Serial.print(" min= ");
    Serial.print(m);
    Serial.print(" seg= ");
    Serial.print(s);

    // Calcula o total em segundos
    total = 3600 * h + 60 * m + s;
    count = total;

    // Exibe o total em segundos no Monitor Serial
    Serial.print(" total em seg= ");
    Serial.println(total);

    // Envia o total em segundos de volta ao dispositivo Bluetooth
    bluetooth.print(" total em seg= ");
    bluetooth.println(total);
  }
  // Decrementa a contagem
  count--;

  // Se a contagem chegar a zero, ativa o buzzer enquanto a distância for maior distancia (cm)
  if (count == 0) {
    do {
      digitalWrite(speaker, HIGH);
      delay(100);
      digitalWrite(speaker, LOW);
      delay(100);
    } while (measureDistance() > distancia);
  }
}
