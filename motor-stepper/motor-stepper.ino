// wait for a single step of stepper
int delaytime = 1;


// ports used to control the stepper motor
// if your motor rotate to the opposite direction,
//change the order as {2, 3, 4, 5};
int port[4] = {11, 10, 9, 8};
int passos = 200;
// sequence of stepper motor control
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

void rotate(int step) {
  static int phase = 0;
  int i, j;

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

void setup() {
  pinMode(port[0], OUTPUT);
  pinMode(port[1], OUTPUT);
  pinMode(port[2], OUTPUT);
  pinMode(port[3], OUTPUT);
}
void loop() {
  for (int i = 0; i < 10; i++) {
    rotate(passos);
  }
  //  delay(1000);
}
