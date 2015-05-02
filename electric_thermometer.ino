#define inPinThermo A0
#define buttonPin 1
#define data 2
#define clock 3
#define firstDigitPin 6
#define secondDigitPin 5
#define thirdDigitPin 4
#define ledPin 7

// use binary notation to discribe our number layouts
byte zero  = B00010001;
byte one   = B01111101;
byte two   = B00100011;
byte three = B00101001;
byte four  = B01001101;
byte five  = B10001001;
byte six   = B10000001;
byte seven = B00111101;
byte eight = B00000001;
byte nine  = B00001001;
byte point = B11111110;

bool overheat;
int temperatureMode;
unsigned long previousMillis;
const long interval = 1000;
float celcius;
int incomingByte = 0;

byte toByte(int digit) {
  switch (digit) {
    case 0:
      return zero;
      break;
    case 1:
      return one;
      break;
    case 2:
      return two;
      break;
    case 3:
      return three;
      break;
    case 4:
      return four;
      break;
    case 5:
      return five;
      break;
    case 6:
      return six;
      break;
    case 7:
      return seven;
      break;
    case 8:
      return eight;
      break;
    case 9:
      return nine;
      break;
    default:
      break;
  }
}

void printSevenSeg(float number) {
  int first_digit;
  int second_digit;
  int third_digit;
  if(number<100) {
    first_digit = ((int)number/10)%10;
    second_digit = (int)number%10;
    third_digit = static_cast<int>(number*10)%10;
    digitalWrite(firstDigitPin, HIGH);
    digitalWrite(secondDigitPin, LOW);
    digitalWrite(thirdDigitPin, LOW);
    shiftOut(data, clock, LSBFIRST, toByte(first_digit));
    delay(7);
    digitalWrite(firstDigitPin, LOW);
    digitalWrite(secondDigitPin, HIGH);
    digitalWrite(thirdDigitPin, LOW);
    shiftOut(data, clock, LSBFIRST, (toByte(second_digit)&point));
    delay(7);
    digitalWrite(firstDigitPin, LOW);
    digitalWrite(secondDigitPin, LOW);
    digitalWrite(thirdDigitPin, HIGH);
    shiftOut(data, clock, LSBFIRST, toByte(third_digit));
    delay(7);
  }
  else {
    first_digit = ((int)number/100)%10;
    second_digit = ((int)number/10)%10;
    third_digit = (int)number%10;
    digitalWrite(firstDigitPin, HIGH);
    digitalWrite(secondDigitPin, LOW);
    digitalWrite(thirdDigitPin, LOW);
    shiftOut(data, clock, LSBFIRST, toByte(first_digit));
    delay(7);
    digitalWrite(firstDigitPin, LOW);
    digitalWrite(secondDigitPin, HIGH);
    digitalWrite(thirdDigitPin, LOW);
    shiftOut(data, clock, LSBFIRST, toByte(second_digit));
    delay(7);
    digitalWrite(firstDigitPin, LOW);
    digitalWrite(secondDigitPin, LOW);
    digitalWrite(thirdDigitPin, HIGH);
    shiftOut(data, clock, LSBFIRST, toByte(third_digit));
    delay(7);
  }
}

float convertSuhu(float suhu, int mode) {
  float conversion;
  switch(mode) {
    case 0: // Celcius
      conversion = suhu;
      break;
    case 1: // Reamur
      conversion = suhu*4/5;
      break;
    case 2: // Kelvin
      conversion = suhu+273;
      break;
    case 3: // Fahrenheit
      conversion = suhu*9/5+32;
      break;
    default:
      conversion = 0;
      break;
  }
  return conversion;
}

void setup()
{
  pinMode(clock, OUTPUT);
  pinMode(data , OUTPUT);
  pinMode(firstDigitPin, OUTPUT);
  pinMode(secondDigitPin, OUTPUT);
  pinMode(thirdDigitPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Hello world!");
  temperatureMode = 0;
  delay(1000);
  previousMillis = 0;
  int value = analogRead(inPinThermo);
  float millivolts = (value / 1024.0) * 100;
  celcius = millivolts * 3.3;
}

void loop() {
  unsigned long currentMillis;
  temperatureMode = temperatureMode%4;
  while(digitalRead(buttonPin) == HIGH) {
    currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      int value = analogRead(inPinThermo);
      float millivolts = (value / 1024.0) * 100;
      celcius = millivolts * 3.3;
    }
    printSevenSeg(convertSuhu(celcius,temperatureMode));
    
    // read input from keyboard
    if (Serial.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial.read();
 
      // say what you got:
      Serial.print("I received: ");
      Serial.println(incomingByte, DEC);
    }
    
    if(celcius>23.0) {
      digitalWrite(ledPin, HIGH);
    }
    else {
      digitalWrite(ledPin, LOW);
    }
  }
  while(digitalRead(buttonPin) == LOW) {
    currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      int value = analogRead(inPinThermo);
      float millivolts = (value / 1024.0) * 100;
      celcius = millivolts * 3.3;
    }
    printSevenSeg(convertSuhu(celcius,temperatureMode));
    
    // read input from keyboard
    if (Serial.available() > 0) {
      // read the incoming byte:
      incomingByte = Serial.read();
 
      // say what you got:
      Serial.print("I received: ");
      Serial.println(incomingByte, DEC);
    }
    
    if(celcius>23.0) {
      digitalWrite(ledPin, HIGH);
    }
    else {
      digitalWrite(ledPin, LOW);
    }
  }
  temperatureMode++;
}

