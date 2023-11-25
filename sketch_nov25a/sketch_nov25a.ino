
#include <Stepper.h>

#define passos 30720  // Número de passos por revolução do motor
#define rpm 60      // Velocidade em rotações por minuto

int port[4] = {2, 3, 4, 5};  // Pinos usados para controlar o motor
int i = 0;

// Cria um objeto da classe Stepper
Stepper motor(passos, port[0], port[1], port[2], port[3]);

void setup() {
  // Define a velocidade em rotações por minuto
  motor.setSpeed(rpm);

  // Configuração dos pinos como saída
  for (i = 0; i < 4; i++) {
    pinMode(port[i], OUTPUT);
  }

  // Inicializa a comunicação serial
  Serial.begin(9600);
}

void loop() {
  
  motor.step(passos);  // O número de passos é igual ao número de passos por revolução
  
}
