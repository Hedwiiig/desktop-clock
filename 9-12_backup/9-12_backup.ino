#include <SoftwareSerial.h>

// Configurações dos alarmes =================================
float alarm_1 = 0;
float test = 0;










#define passos 8  // Número de passos por revolução do motor
int port[4] = {8, 9, 10, 11};  // Pinos usados para controlar o motor
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
#define rx 6
#define tx 5

char select_time = 's'; 
int h = -1, m = -1, s = -1;
int total = 0;
SoftwareSerial bluetooth(rx, tx); // RX, TX
String minutos_str;
String horas_str;
String segundos_str;


int select_time_func() {
  // Verifica se há dados disponíveis no módulo Bluetooth
  
    // Ler valor de horas
    while(!bluetooth.available() || h < 0 || h != 0){
    rotacao(passos);
    bluetooth.print("horas= ");
    horas_str = bluetooth.readStringUntil('\n');
    if(horas_str != "")
    rotacao(passos);
    {
    if(h > 0 || h == 0  )break;
    h = horas_str.toInt(); // Converte o valor de horas para inteiro
    bluetooth.println("");
    rotacao(passos);
    }
    } // Envia uma quebra de linha

    // Ler valor de minutos
    while(!bluetooth.available() || m < 0 || m != 0)
    {
    if(m > 0 || m == 0)break;
    rotacao(passos);
    bluetooth.print("minutos= ");
    minutos_str = bluetooth.readStringUntil('\n');
    rotacao(passos);
    if(minutos_str != "" )
    {
    m = minutos_str.toInt(); // Converte o valor de minutos para inteiro esta dentro do if pois h,m,s sao inicialzados a -1 e esta funçao coloca os a 1 logo faz com que saiam do loop
    bluetooth.println("");
    rotacao(passos);
    }
    } // Envia uma quebra de linha

    // Ler valor de segundos
    while(!bluetooth.available() || s < 0 || s != 0){
    if(s > 0 || s == 0)break;
    rotacao(passos);
    bluetooth.print("segundos= ");
    segundos_str = bluetooth.readStringUntil('\n');
    rotacao(passos);
    if(segundos_str != "" )
    {
    s = segundos_str.toInt(); // Converte o valor de segundos para inteiro
    bluetooth.println("");
    rotacao(passos);
    }
    } // Envia uma quebra de linha

    // Exibe os valores lidos no Monitor Serial
    Serial.print("horas= ");
    Serial.print(h);
    Serial.print(" min= ");
    Serial.print(m);
    Serial.print(" seg= ");
    Serial.print(s);

    // Calcula o total em segundos
    total = 3600 * h + 60 * m + s;

    // Envia o total em segundos de volta ao dispositivo Bluetooth
    bluetooth.print(" total em seg= ");
    bluetooth.print(total);
    h = -1;
    m = -1;
    s = -1;
    segundos_str = "";
    minutos_str = "" ;
    horas_str = "";


    return(total); 
}

// Configurações do buzzer =========================================
bool count = false,play = true;
#define speaker 4
float speaker_d = 0;

// Configurações do Sensor Ultrassônico HC-SR04 =====================
# define trigPin 2
# define echoPin 3
int distancia = 5;

// Função para medir a distância com o sensor ultrassônico
float measureDistance() {
  float duration, distance;

  // Envia pulso
  rotacao(passos);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  rotacao(passos);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Aguarda o eco e mede o tempo até que ocorra
  duration = pulseIn(echoPin, HIGH);
  rotacao(passos);

  // Calcula a distância
  distance = (duration * 0.0343) / 2;
  rotacao(passos);

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
  bluetooth.begin(38400);
  
}

void loop() {
  
  rotacao(passos);
  
  if(bluetooth.available() && bluetooth.read() == select_time)
  {
    rotacao(passos);
    alarm_1 = select_time_func();
    test = millis() + (alarm_1*1000);
    play = true;
    rotacao(passos);
    
  }
  if(((test - millis()) < 0) && alarm_1 != 0) count = play;
  rotacao(passos);
  Serial.println(measureDistance());
  float ts = millis();
  
  


  if (count == true) {
    do {
    if((millis() - ts) < 100)
      {
        digitalWrite(speaker, HIGH);
        rotacao(passos);
      }

      if((millis() - ts) > 100)
      {
        digitalWrite(speaker, LOW);
        rotacao(passos);
        ts = millis();
      }
    speaker_d = measureDistance();
     if(speaker_d < distancia)
     {
      digitalWrite(speaker,LOW);
     
      count = !play;
      play = false;
      break;
     }
      
    } while (speaker_d > distancia);
  }
}
