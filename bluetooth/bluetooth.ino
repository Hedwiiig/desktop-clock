#include <SoftwareSerial.h>

#define rx 0
#define tx 1

unsigned int h, m, s;
unsigned int total;
SoftwareSerial bluetooth(rx, tx); // RX, TX

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
}

void loop() {
  // Verifica se há dados disponíveis no módulo Bluetooth
  if (bluetooth.available()) {
    // Ler valor de horas
    bluetooth.print("horas= ");
    String horas_str = bluetooth.readStringUntil('\n');
    h = horas_str.toInt(); // Converte o valor de horas para inteiro
    bluetooth.println(""); // Envia uma quebra de linha

    // Ler valor de minutos
    bluetooth.print("minutos= ");
    String minutos_str = bluetooth.readStringUntil('\n');
    m = minutos_str.toInt(); // Converte o valor de minutos para inteiro
    bluetooth.println(""); // Envia uma quebra de linha

    // Ler valor de segundos
    bluetooth.print("segundos= ");
    String segundos_str = bluetooth.readStringUntil('\n');
    s = segundos_str.toInt(); // Converte o valor de segundos para inteiro
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

    // Exibe o total em segundos no Monitor Serial
    Serial.print(" total em seg= ");
    Serial.println(total);

    // Envia o total em segundos de volta ao dispositivo Bluetooth
    bluetooth.print(" total em seg= ");
    bluetooth.println(total);
  }
}
