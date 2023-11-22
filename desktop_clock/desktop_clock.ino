#define MILLIS_PER_SEG 1000 // Definir como 1000 para representar segundos

// Motor and clock parameters
// 4096 * 90 / 12 = 30720
#define STEPS_PER_ROTATION 30720 // steps for a full turn of minute rotor

// wait for a single step of stepper
int delaytime = 2;
long min = 0; // Mudar de millis para minutos
long sec = 0; // Adicionar uma variável para os segundos
// ports used to control the stepper motor
// if your motor rotate to the opposite direction,
// change the order as {2, 3, 4, 5};
int port[4] = {5, 4, 3, 2};

// sequence of stepper motor control
int seq[8][4] = {
  {  LOW, HIGH, HIGH,  LOW},
  {  LOW,  LOW, HIGH,  LOW},
  {  LOW,  LOW, HIGH, HIGH},
  {  LOW,  LOW,  LOW, HIGH},
  { HIGH,  LOW,  LOW, HIGH},
  { HIGH,  LOW,  LOW,  LOW},
  { HIGH, HIGH,  LOW,  LOW},
  {  LOW, HIGH,  LOW,  LOW}
};

void rotate(int step) {
  static int phase = 0;
  int i, j;
  int delta = (step > 0) ? 1 : 7;
  int dt = 20;

  step = (step > 0) ? step : -step;
  for (j = 0; j < step; j++) {
    phase = (phase + delta) % 8;
    for (i = 0; i < 4; i++) {
      digitalWrite(port[i], seq[phase][i]);
    }
    delay(dt);
    if (dt > delaytime) dt--;
  }
  // power cut
  for (i = 0; i < 4; i++) {
    digitalWrite(port[i], LOW);
  }
}

void setup() {
  pinMode(port[0], OUTPUT);
  pinMode(port[1], OUTPUT);
  pinMode(port[2], OUTPUT);
  pinMode(port[3], OUTPUT);
  rotate(-20); // for approach run
  rotate(20); // approach run without heavy load
  rotate(STEPS_PER_ROTATION / 60);
}

void loop() {
  static long prev_min = 0, prev_sec = 0;
  // Atualizar a variável min para representar minutos
  min = millis() / MILLIS_PER_SEG;

  if (prev_min == min) {
    return;
  }

  prev_min = min;
  // Atualizar a variável sec para representar segundos
  sec = (min * 60) % 60;

  rotate(-20); // para execução de aproximação
  rotate(20); // execução de aproximação sem carga pesada

  // Girar para a nova posição representada pelos segundos
  if (sec - prev_sec > 0) {
    rotate(sec - prev_sec);
  }

  prev_sec = sec;
}
