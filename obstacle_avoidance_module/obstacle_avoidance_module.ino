// === Pines de MOTORES (L9110S), iguales en ambos ejemplos ===
const int A_1B = 5;
const int A_1A = 6;
const int B_1B = 9;
const int B_1A = 10;

// === Pines de SENSORES (mantengo los tuyos) ===
// IR laterales
const int rightIR = 7;  // 0: obstáculo, 1: libre
const int leftIR  = 8;  // 0: obstáculo, 1: libre
// Ultrasonido
const int trigPin = 3;
const int echoPin = 4;

// === Parámetros de comportamiento ===
const int SPEED_FWD  = 150;   // velocidad avance
const int SPEED_BACK = 150;   // velocidad retroceso/pivote
const int D_STOP     = 25;    // umbral "libre" por ultrasónico (> D_STOP = libre)
const int D_MIN      = 2;     // descarta ecos inválidos (<2 cm)

void setup() {
  Serial.begin(9600);

  // Motores
  pinMode(A_1B, OUTPUT);
  pinMode(A_1A, OUTPUT);
  pinMode(B_1B, OUTPUT);
  pinMode(B_1A, OUTPUT);

  // IR laterales
  pinMode(leftIR,  INPUT);
  pinMode(rightIR, INPUT);

  // Ultrasonido
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  stopMove();  // arranca quieto
}

void loop() {
  // Lecturas
  int left  = digitalRead(leftIR);   // 0: obstáculo, 1: libre
  int right = digitalRead(rightIR);  // 0: obstáculo, 1: libre
  float d   = readDistanceCM();

  // Diagnóstico (opcional)
  // Serial.print("d="); Serial.print(d); Serial.print(" L="); Serial.print(left); Serial.print(" R="); Serial.println(right);

  bool ultraLibre   = (d > D_STOP) || (d < D_MIN); // si no hay eco válido, no bloquea
  bool lateralesOK  = (left == 1 && right == 1);   // ambos libres

  if (ultraLibre && lateralesOK) {
    // Camino despejado: avanzar recto
    moveForward(SPEED_FWD);
  } else {
    // Hay obstáculo por cualquiera de los sensores -> maniobra evasiva
    // 1) Retrocede suave
    moveBackward(SPEED_BACK);
    delay(250);

    // 2) Elige lado: si el izquierdo ve obstáculo (0) y el derecho está libre (1),
    // pivotea hacia la derecha; y viceversa. Si ambos ven obstáculo (o ambos libres pero ultra cerca),
    // pivotea a la derecha por defecto.
    if (left == 0 && right == 1) {
      backRight(SPEED_BACK);    // gira/pivotea hacia derecha
    } else if (left == 1 && right == 0) {
      backLeft(SPEED_BACK);     // gira/pivotea hacia izquierda
    } else {
      backRight(SPEED_BACK);    // por defecto
    }
    delay(250);

    // 3) Pausa corta y vuelve a evaluar en el próximo ciclo
    stopMove();
    delay(80);
  }
}

/* ----------------- FUNCIONES DE MOTORES (sin redundancias) ----------------- */
void moveForward(int speed) {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, speed);
  analogWrite(B_1B, speed);
  analogWrite(B_1A, 0);
}

void moveBackward(int speed) {
  analogWrite(A_1B, speed);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, speed);
}

// Pivoteo en retroceso hacia la IZQUIERDA (rueda derecha retrocede, izquierda parada)
void backLeft(int speed) {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, speed);
}

// Pivoteo en retroceso hacia la DERECHA (rueda izquierda retrocede, derecha parada)
void backRight(int speed) {
  analogWrite(A_1B, speed);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, 0);
}

void stopMove() {
  analogWrite(A_1B, 0);
  analogWrite(A_1A, 0);
  analogWrite(B_1B, 0);
  analogWrite(B_1A, 0);
}

/* ----------------- ULTRASONIDO ----------------- */
float readDistanceCM() {
  // Disparo
  digitalWrite(trigPin, LOW);  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Eco (conversión a cm: us/58)
  long dur = pulseIn(echoPin, HIGH, 22000UL); // timeout ~3.8 m para evitar bloqueos
  if (dur <= 0) return 0;      // sin eco válido
  return dur / 58.0;
}
