// =========================================================================
// RECTIFICADOR TRIFÁSICO CONTROLADO - ARDUINO
// Cruce por cero: Pin 2 (INT0)
// SCR: Pines 8, 9, 10
// Ángulo de disparo definido por constante
// =========================================================================

// ===============================
// DEFINICIONES DE PINES
// ===============================
#define ZERO_CROSS_PIN  2
#define SCR1_PIN        8
#define SCR2_PIN        9
#define SCR3_PIN        10

// ===============================
// CONSTANTES DE TIEMPO
// ===============================

// Ángulo de disparo (definido como retardo en microsegundos)
#define FIRING_DELAY_US   3688 // corresponde aprox. a 80°–90° a 60 Hz

// Desfase entre fases (120° en 60 Hz)
#define PHASE_SHIFT_US    5556

// Ancho del pulso de compuerta del SCR
#define PULSE_WIDTH_US   6916 - FIRING_DELAY_US 

// ===============================
// VARIABLES GLOBALES
// ===============================
volatile bool newZeroCross = false;

// ===============================
// INTERRUPCIÓN DE CRUCE POR CERO
// ===============================
void zeroCrossISR()
{
  newZeroCross = true;
}

// ===============================
// SETUP
// ===============================
void setup()
{
  // Configurar salidas de SCR
  pinMode(SCR1_PIN, OUTPUT);
  pinMode(SCR2_PIN, OUTPUT);
  pinMode(SCR3_PIN, OUTPUT);

  digitalWrite(SCR1_PIN, LOW);
  digitalWrite(SCR2_PIN, LOW);
  digitalWrite(SCR3_PIN, LOW);

  // Configurar entrada de cruce por cero
  pinMode(ZERO_CROSS_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(ZERO_CROSS_PIN), zeroCrossISR, RISING);
}

// ===============================
// LOOP PRINCIPAL
// ===============================
void loop()
{
  if (newZeroCross)
  {
    newZeroCross = false;
    triggerSCRSequence();
  }
}

// ===============================
// SECUENCIA DE DISPARO
// ===============================
void triggerSCRSequence()
{
  // ---- FASE A ----
  delayMicroseconds(FIRING_DELAY_US);

  digitalWrite(SCR1_PIN, HIGH);
  delayMicroseconds(PULSE_WIDTH_US);
  digitalWrite(SCR1_PIN, LOW);

  // ---- FASE B (120° después) ----
  delayMicroseconds(PHASE_SHIFT_US);

  digitalWrite(SCR2_PIN, HIGH);
  delayMicroseconds(PULSE_WIDTH_US);
  digitalWrite(SCR2_PIN, LOW);

  // ---- FASE C (240° después) ----
  delayMicroseconds(PHASE_SHIFT_US);

  digitalWrite(SCR3_PIN, HIGH);
  delayMicroseconds(PULSE_WIDTH_US);
  digitalWrite(SCR3_PIN, LOW);
}