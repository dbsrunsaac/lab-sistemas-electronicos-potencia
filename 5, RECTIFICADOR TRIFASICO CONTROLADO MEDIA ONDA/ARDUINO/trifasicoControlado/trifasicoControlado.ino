/*
 Control de disparo de SCR trifásico usando detector de cruce por cero

 Pin 2  -> Entrada cruce por cero (PC817)
 Pin 8  -> Disparo SCR fase U
 Pin 9  -> Disparo SCR fase V
 Pin 10 -> Disparo SCR fase W
 A0     -> Potenciómetro para ajustar ángulo de disparo (0° a 110°)
*/

const int zeroCrossPin = 2;

const int SCR_U = 8;
const int SCR_V = 9;
const int SCR_W = 10;

const int potPin = A0;

volatile bool zeroCrossDetected = false;

unsigned long zeroTime = 0;

// parámetros del sistema
const float frecuencia = 60.0;
const float semiPeriodo_us = 8333;   // medio ciclo en microsegundos
const float fase120_us = 5533;       // separación entre fases (120°)

int angulo = 0;
unsigned long delay_disparo = 0;

void setup()
{
  pinMode(zeroCrossPin, INPUT);

  pinMode(SCR_U, OUTPUT);
  pinMode(SCR_V, OUTPUT);
  pinMode(SCR_W, OUTPUT);

  digitalWrite(SCR_U, LOW);
  digitalWrite(SCR_V, LOW);
  digitalWrite(SCR_W, LOW);

  attachInterrupt(digitalPinToInterrupt(zeroCrossPin), zeroCrossISR, RISING);

  Serial.begin(9600);
}

void loop()
{
  // Leer potenciometro y convertir a angulo (0° a 110°)
  int potValue = analogRead(potPin);
  angulo = map(potValue, 0, 1023, 0, 110);
  Serial.println(angulo);

  // convertir angulo a tiempo
  delay_disparo = angulo*16667/360;

  if (zeroCrossDetected)
  {
    zeroCrossDetected = false;

    // tiempo base desde cruce por cero
    unsigned long t0 = micros();

    // tiempos de disparo de cada fase
    unsigned long tU = t0 + 1383 + delay_disparo;
    unsigned long tV = t0 + 1383 + delay_disparo + fase120_us;
    unsigned long tW = t0 + 1383 + delay_disparo + (2 * fase120_us);

    dispararSCR(SCR_U, tU);
    dispararSCR(SCR_V, tV);
    dispararSCR(SCR_W, tW);
    Serial.println("SRC disparados");
  }
}

void dispararSCR(int pin, unsigned long tiempoDisparo)
{
  while (micros() < tiempoDisparo);

  digitalWrite(pin, HIGH);
  delayMicroseconds(100);   // pulso de compuerta
  digitalWrite(pin, LOW);
}

void zeroCrossISR()
{
  zeroCrossDetected = true;
}

