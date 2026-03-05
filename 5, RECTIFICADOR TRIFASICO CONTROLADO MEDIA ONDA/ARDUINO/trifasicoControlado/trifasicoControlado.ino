const int led =4;
const byte entCero = 2;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(entCero), activarLed, RISING);
  
  Serial.println("Programa iniciado");
}

void loop() {
  digitalWrite(led, LOW);
  delay(250);
}

void activarLed(){
  digitalWrite(led, HIGH);
  Serial.println("Cre ro detectado");
}
