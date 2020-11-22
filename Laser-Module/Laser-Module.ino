int laserPin = 10;

void setup ()
{
  pinMode(laserPin, OUTPUT);
  Serial.begin(9600);
}

void loop () {
  digitalWrite(laserPin, HIGH);
  Serial.println("high");
  delay(100);
  digitalWrite(laserPin, LOW);
  Serial.println("low");
  delay(100);
}
