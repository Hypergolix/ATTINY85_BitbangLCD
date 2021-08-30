// Not a library but uses a HC595 SIPO to communicate/bitbang to a HD44780s data lines for lowered pincount

// Known bugs: LCD may initialise with garbage unless cleared. And speed could be improved

// Most pins are reusable as long as they are returned to default state after 2nd use/ before primary use
// PINs: 4, 2, 0
#define PB3 3                               // Display enable - non
#define PB4 4                               // Register select - reusable 
// Not necessary but for readability
#define CMND 0
#define DATA 1

char wordArr[] = "Started!";
char wordArr2[] = "Display!";

// Delay in microseconds, 1000us = 1ms. Overflow during a delay?
void microDelay(unsigned long duration) {
  unsigned long initialTime = micros();     // Grab the starting time
  while (duration > (micros() - initialTime));
}

// Driving the display too fast?

// Write character
void LCDWrite (uint8_t Data, bool Register) {
  digitalWrite(PB4, Register);              // Select data or command register
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(2, LOW);
    digitalWrite(0, Data & (128 >> i));     // Start with MSB
    digitalWrite(2, HIGH);
    microDelay(10);                         // 10us, could reduce?
    digitalWrite(2, LOW);                   // Clk low/idle
    digitalWrite(0, LOW);                   // MOSI low/idle
  }
  // Extra clock - repeat
  digitalWrite(2, HIGH);
  microDelay(10);
  digitalWrite(2, LOW);
  // Clock in data to LCD
  digitalWrite(PB3, HIGH);
  microDelay(10);
  digitalWrite(PB3, LOW);
  digitalWrite(PB4, LOW);
}

void setup() {
  for (uint8_t i = 0; i < 6; i++) {
    pinMode(i, OUTPUT);                     // Set all pins output
    digitalWrite(i, LOW);                   // set all pins to idle low
  }
  // LCD Set-Up - was clear display really needed before?
  LCDWrite(0x0F, CMND);                     // Turns LCD Display ON (both rows?)
  //LCDWrite(0x06, CMND);                     // Cursor behaviour
  LCDWrite(0x01, CMND);                     // Clear display
  //LCDWrite(0x02, CMND);                     // Return to home, unshift
  //LCDWrite(0x80, CMND);
  for (uint8_t i = 0; wordArr[i] != 0; i++) {
    LCDWrite(wordArr[i], DATA);             // Write word to display
  }
  
  /*
  LCDWrite(0xC0, CMND);                     // Cursor + position
  for (uint8_t i = 0; wordArr2[i] != 0; i++) {
    LCDWrite(wordArr2[i], DATA);            // Write word to display
  }
  for (uint8_t i = 0; i < 256; i++) {
    // LCDWrite('A', DATA);
    //LCDWrite(0x18, CMND);
    LCDWrite(0x10, CMND);
    microDelay(250000);
  }
  */
}

void loop() {
  delay(1000);
}
