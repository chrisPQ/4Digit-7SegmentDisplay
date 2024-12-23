
//Pin connected to ST_CP of 74HC595
int latchPin = 3;
//Pin connected to SH_CP of 74HC595
int clockPin = 2;
////Pin connected to DS of 74HC595
int dataPin = 4;

const int button1Pin = 6;
const int button2Pin = 7;
const int ledPin = 13;
const int dig1 = 10; // top left on 4 seg
const int dig2 = 5; // bottom right
const int dig3 = 8; // far right on sev seg
const int dig4 = 9; // next to right of ten pin above

int button1State;
int previous1State = LOW;
int button2State;
int previous2State = LOW;

int pushUpCount = 0; //number to display

unsigned long last1Debounce = 0;
unsigned long debounce1Time = 50; // in ms

unsigned long last2Debounce = 0;
unsigned long debounce2Time = 50; // in ms

                //number array (1 is segment on)
                //ABCDEFGX
                //0         1           2         3           4           5           6           7           8           9
int arr[10] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11110110};


void setup() {
          //set pins to output so you can control the shift register and led digits
          pinMode(latchPin, OUTPUT);
          pinMode(clockPin, OUTPUT);
          pinMode(dataPin, OUTPUT);
          pinMode(button1Pin, INPUT);
          pinMode(button2Pin, INPUT);
          pinMode(dig1, OUTPUT);
          pinMode(dig2, OUTPUT);
          pinMode(dig3, OUTPUT);
          pinMode(dig4, OUTPUT);
          pinMode(ledPin, OUTPUT);
          digitalWrite(ledPin, LOW);

           
}

void loop() {
  
  int button1Reading = digitalRead(button1Pin);
  int button2Reading = digitalRead(button2Pin);

  //debounce saving time
  if(button1Reading != previous1State) {
    last1Debounce = millis();
  }
  
  if(button2Reading != previous2State) {
    last2Debounce = millis();
  }

  // increase with one button and check for debounce
  if((millis() - last1Debounce) > debounce1Time) {
    if(button1Reading != button1State) {
      button1State = button1Reading;
        if(button1State == HIGH) {
          pushUpCount += 5;
          if(pushUpCount >= 1000) {
            pushUpCount = 1000;
          }
        }
      }
  }

  // decrease with other button
  if((millis() - last2Debounce) > debounce2Time) {
    if(button2Reading != button2State) {
      button2State = button2Reading;
        if(button2State == HIGH) {
          pushUpCount -= 5;
          if(pushUpCount <= 0) {
            pushUpCount = 0;
          }
        }
      }
  }

  //save previous state of button if debounce isn't high enough
  previous1State = button1Reading;
  
  previous2State = button2Reading;

  //display all digits with the count
  displayDigits(pushUpCount);
}

void displaySegment(int number) {
             // take the latchPin low so
            // the LEDs don't change while you're sending in bits:
            digitalWrite(latchPin, LOW);
            // shift out the bits:
            shiftOut(dataPin, clockPin, LSBFIRST, arr[number]);
            //take the latch pin high so the LEDs will light up:
            digitalWrite(latchPin, HIGH);
}

int returnDigitNum(int number, int index) {
  //divide by index power of 10 and take the remainder and return it to get num 0-9
  return (number / (int)pow(10, index)) % 10;
}

void displayDigits(int number) {
          // activate one pin
           digitalWrite(dig1, LOW);
          // display number and deactivate
           displaySegment(returnDigitNum(number,0));
           digitalWrite(dig1, HIGH);
           
          // activate one pin
           digitalWrite(dig2, LOW);
          // display number and deactivate
           displaySegment(returnDigitNum(number,1));
           digitalWrite(dig2, HIGH);
           
          // activate one pin
           digitalWrite(dig3, LOW);
          // display number and deactivate
           displaySegment(returnDigitNum(number,2));
           digitalWrite(dig3, HIGH);
           
           // activate one pin
           digitalWrite(dig4, LOW);
          // display number and deactivate
           displaySegment(returnDigitNum(number,3));
           digitalWrite(dig4, HIGH);
}
