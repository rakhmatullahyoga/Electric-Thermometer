#define inPinThermo A0
#define buttonPin1 2
#define buttonPin2 3
#define firstDigitPin 4
#define secondDigitPin 5
#define thirdDigitPin 6
#define data 7
#define clock 8
#define ledPin 9
#define motorPin 10

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

int temperatureMode;
unsigned long previousMillis;
const long interval = 500;
float celcius;
int increment;
int incomingByte = 0;
int fadeValue;
int prevMillisLED;
bool inputMode;
float overheatPoint;

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
    delay(5);
    digitalWrite(firstDigitPin, LOW);
    digitalWrite(secondDigitPin, HIGH);
    digitalWrite(thirdDigitPin, LOW);
    shiftOut(data, clock, LSBFIRST, toByte(second_digit));
    delay(5);
    digitalWrite(firstDigitPin, LOW);
    digitalWrite(secondDigitPin, LOW);
    digitalWrite(thirdDigitPin, HIGH);
    shiftOut(data, clock, LSBFIRST, toByte(third_digit));
    delay(5);
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

void changeTemperatureMode() {
  Serial.println(temperatureMode);
  while(digitalRead(buttonPin1) == LOW);
  temperatureMode++;
  temperatureMode = temperatureMode%4;
}

void inputOverheat() {
  while(digitalRead(buttonPin2) == LOW) {
    printSevenSeg(convertSuhu(celcius,temperatureMode));
  }
  Serial.println("Masukkan overheat point baru!");
  inputMode = !inputMode;
}

void setup()
{
  attachInterrupt(0, changeTemperatureMode, LOW);
  attachInterrupt(1, inputOverheat, LOW);
  pinMode(motorPin, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data , OUTPUT);
  pinMode(firstDigitPin, OUTPUT);
  pinMode(secondDigitPin, OUTPUT);
  pinMode(thirdDigitPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Hello world!");
  temperatureMode = 0;
  previousMillis = 0;
  celcius = 0;
  fadeValue = 0;
  overheatPoint = 27;
  inputMode = false;
  delay(1000);
}

void loop() {
  unsigned long currentMillis;
  
  if(inputMode) {
    while(Serial.available() <= 0) {
      printSevenSeg(convertSuhu(celcius,temperatureMode));
    }
    if (Serial.available() > 0) {
      // read the incoming byte:
      float input = Serial.parseFloat();
 
      // say what you got:
      overheatPoint = input;
      Serial.print("Batas overheat baru: ");
      Serial.println(overheatPoint);
    }
    inputMode = false;
  }
  else {
    currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      int value = analogRead(inPinThermo);
      float millivolts = (value / 1024.0) * 5000;
      celcius = millivolts / 10 - 3;
    }
    
    // Overheat
    if(celcius>overheatPoint) {
      digitalWrite(motorPin, HIGH);
      int ledInterval = 10;
      
      fadeValue += increment;
      prevMillisLED = currentMillis;
      if(currentMillis - prevMillisLED >= ledInterval) {
        prevMillisLED = currentMillis;
      }
      if(!(fadeValue>0&&fadeValue<255)) {
        increment = -increment;
      }
    }
    else {
      digitalWrite(motorPin, LOW);
      fadeValue = LOW;
      increment = 5;
    }
  }
  printSevenSeg(convertSuhu(celcius,temperatureMode));
  analogWrite(ledPin, fadeValue);
}

