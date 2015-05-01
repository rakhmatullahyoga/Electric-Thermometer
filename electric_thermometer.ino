#define data 2
#define clock 3
#define second_digit 4
#define first_digit 5

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

int i;

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
  }
}

void printSevenSeg(int number) {
  int puluhan = number/10;
  int satuan = number%10;
  
  for(int iter=0; iter<50; iter++) {
    digitalWrite(first_digit, HIGH);
    digitalWrite(second_digit, LOW);
    shiftOut(data, clock, LSBFIRST, toByte(puluhan));
    delay(10);
    digitalWrite(first_digit, LOW);
    digitalWrite(second_digit, HIGH);
    shiftOut(data, clock, LSBFIRST, toByte(satuan));
    delay(10);
  }
}

void setup()
{
  pinMode(clock, OUTPUT);
  pinMode(data , OUTPUT);
  pinMode(second_digit, OUTPUT);
  pinMode(first_digit, OUTPUT);
  i = 0;
}

void loop()
{
//  printSevenSeg(i);
//  i++;
}

