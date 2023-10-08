// Allows to use SET() and CLR() instead of digitalWrite(), making
// code A LOT faster
// https://www.arduino.cc/en/Reference/PortManipulation
// http://www.optiloading.be/willem/Arduino/speeding.pdf
#define CLR(x, y) (x&=(~(1<<y)))
#define SET(x, y) (x|=(1<<y))

// Pins for shift register controlling rows
const int latchPinRow = 13;
const int clockPinRow = 12;
const int dataPinRow = 11;

// Pins for shift register controlling columns for first letter
const int latchPinColumn1 = 7;
const int clockPinColumn1 = 4;
const int dataPinColumn1 = 3;
// Pins for shift register controlling columns for second letter
const int latchPinColumn2 = 6;
const int clockPinColumn2 = 2;
const int dataPinColumn2 = A0;
// Pins for shift register controlling columns for third letter
const int latchPinColumn3 = 5;
const int clockPinColumn3 = A1;
const int dataPinColumn3 = A2;

byte selectedChars[3][7];

byte chars[][7] = { // Source: https://www.pinterest.ca/pin/817614507337504806/
{ // A
  0b01110000, 0b10001000, 0b10001000, 0b11111000, 0b10001000, 0b10001000, 0b10001000},
{ // B
  0b11110000, 0b10001000, 0b10001000, 0b11110000, 0b10001000, 0b10001000, 0b11110000},
{ // C
  0b01110000, 0b10001000, 0b10000000, 0b10000000, 0b10000000, 0b10001000, 0b01110000},
{ // D
  0b11110000, 0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b11110000},
{ // E
  0b11111000, 0b10000000, 0b10000000, 0b11110000, 0b10000000, 0b10000000, 0b11111000},
{ // F
  0b11111000, 0b10000000, 0b10000000, 0b11110000, 0b10000000, 0b10000000, 0b10000000},
{ // G
  0b01110000, 0b10001000, 0b10000000, 0b10111000, 0b10001000, 0b10001000, 0b01110000},
{ // H
  0b10001000, 0b10001000, 0b10001000, 0b11111000, 0b10001000, 0b10001000, 0b10001000},
{ // I
  0b01110000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b01110000},
{ // J
  0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b10001000, 0b10001000, 0b01110000},
{ // K
  0b10001000, 0b10010000, 0b10100000, 0b11000000, 0b10100000, 0b10010000, 0b10001000},
{ // L
  0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b11111000},
{ // M
  0b10001000, 0b11011000, 0b10101000, 0b10101000, 0b10001000, 0b10001000, 0b10001000},
{ // N
  0b10001000, 0b11001000, 0b10101000, 0b10011000, 0b10001000, 0b10001000, 0b10001000},
{ // O
  0b01110000, 0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b01110000},
{ // P
  0b11110000, 0b10001000, 0b10001000, 0b11110000, 0b10000000, 0b10000000, 0b10000000},
{ // Q
  0b01110000, 0b10001000, 0b10001000, 0b10001000, 0b10101000, 0b10010000, 0b11101000},
{ // R
  0b11110000, 0b10001000, 0b10001000, 0b11110000, 0b10100000, 0b10010000, 0b10001000},
{ // S
  0b01110000, 0b10001000, 0b10000000, 0b01110000, 0b00001000, 0b10001000, 0b01110000},
{ // T
  0b11111000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000},
{ // U
  0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b01110000},
{ // V
  0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b10001000, 0b01010000, 0b00100000},
{ // W
  0b10001000, 0b10001000, 0b10001000, 0b10101000, 0b10101000, 0b11011000, 0b10001000},
{ // X
  0b10001000, 0b10001000, 0b01010000, 0b00100000, 0b01010000, 0b10001000, 0b10001000},
{ // Y
  0b10001000, 0b10001000, 0b10001000, 0b01010000, 0b00100000, 0b00100000, 0b00100000},
{ // Z
  0b11111000, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b11111000},
{ // ?
  0b01110000, 0b10001000, 0b00001000, 0b00010000, 0b00100000, 0b00000000, 0b00100000},
{ // !
  0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b00000000, 0b00100000},
{ // 1
  0b00100000, 0b01100000, 0b00100000, 0b00100000, 0b00100000, 0b00100000, 0b01110000},
{ // 2
  0b01110000, 0b10001000, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b11111000},
{ // 3
  0b11110000, 0b00001000, 0b00001000, 0b01110000, 0b00001000, 0b00001000, 0b11110000},
{ // 4
  0b10001000, 0b10001000, 0b10001000, 0b11111000, 0b00001000, 0b00001000, 0b00001000},
{ // 5
  0b11111000, 0b10000000, 0b10000000, 0b11110000, 0b00001000, 0b00001000, 0b11110000},
{ // 6
  0b01110000, 0b10000000, 0b10000000, 0b11110000, 0b10001000, 0b10001000, 0b01110000},
{ // 7
  0b11111000, 0b00001000, 0b00001000, 0b00010000, 0b00100000, 0b00100000, 0b00100000},
{ // 8
  0b01110000, 0b10001000, 0b10001000, 0b01110000, 0b10001000, 0b10001000, 0b01110000},
{ // 9
  0b01110000, 0b10001000, 0b10001000, 0b01111000, 0b00001000, 0b10001000, 0b01110000},
{ // 0
  0b01110000, 0b10001000, 0b10011000, 0b10101000, 0b11001000, 0b10001000, 0b01110000},
{ // Space
  0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0}
};


void updateShiftRegisterRow(byte inputBit) {
   // This function is only for the rows
   // Set latch pin to LOW and call shiftOut method, it's a 
   // built-in function meant for the shift register first two
   // parameters are the two clock pins and the last parameter is 
   // the byte it will be writing to the shift register; LSBFIRST
   // stands for 'Least Significant Bit', meaning we read the byte
   // right-to-left. 
   // https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftout/
   CLR(PORTB, 5);
   shiftOut(dataPinRow, clockPinRow, LSBFIRST, inputBit);
   SET(PORTB, 5);
}

void updateShiftRegisterColumn(byte inputByte, int latchPin, int clockPin, int dataPin){  
	// This function works similarily to updateShiftRegisterRow()
    // except it takes an entire byte as input, (other function only
  	// reads the first bit of the byte)
    byte inputBit = 0;
    for (int i = 0; i <= 8; i++) { // loop through every bit in inputByte
     CLR(PORTD, latchPin); 
      if (bitRead(inputByte, i) == 1) {
        inputBit = 0b10000000;
      } else {
      	inputBit = 0b0;
      }
      shiftOut(dataPin, clockPin, LSBFIRST, inputBit);
      SET(PORTD, latchPin); 
    }
}

void displayChar(byte letter1[5], byte letter2[5], byte letter3[5]) {
  // Set each row to HIGH, then check 'letter' to see if column
  // should also be turned on, keep on looping through everything
  // really fast to give the affect that there are multiple LEDs 
  // when in actuality there is only one on at a time
  
  // Use shift register to loop through rows
  byte rowByte = 0; // Define byte for 'updateShiftRegister'
  for (int i = 0; i <= 6; i++) { // There are 6 rows
    // Set 'i'th bit to 1 
    // bitSet() reads right to left
    bitSet(rowByte, i); 
    updateShiftRegisterRow(rowByte); 
    
    // Shift register has all HIGH pins on at the same time, 
    // decreasing brightness, turns one 1 byte with 8 values
    // into 8 bytes with 1 however it slows down the code A LOT
    for (int j = 0; j <= 7; j++) {
      byte currentBit = 0b0; 
      if (bitRead(letter1[i], j) == 1) { // This skips all the 0 values in the byte, speeding up the code
        bitSet(currentBit, j);
          
        updateShiftRegisterColumn(currentBit, latchPinColumn1, clockPinColumn1, dataPinColumn1);
      	// When using a shift register, as the bits move to their place
        // they light up LEDs that shouldn't be lit, so I added a 
        // transistor to to make sure no LEDs turn on until all the 
        // correct bits are in place
        SET(PORTB, 2);
        CLR(PORTB, 2);
        currentBit = 0b0;
      }
      if (bitRead(letter2[i], j) == 1) { 
        bitSet(currentBit, j);
          
        updateShiftRegisterColumn(currentBit, latchPinColumn2, clockPinColumn2, dataPinColumn2);
        SET(PORTB, 1);
        CLR(PORTB, 1);
        currentBit = 0b0;
      }
      if (bitRead(letter3[i], j) == 1) { 
        bitSet(currentBit, j);
          
        updateShiftRegisterColumn(currentBit, latchPinColumn3, clockPinColumn3, dataPinColumn3);
        SET(PORTB, 0);
        CLR(PORTB, 0);
      }
    }
    
  }
  updateShiftRegisterRow(0b11111111); // I forgot what this does, but everything breaks if I remove it
}

void getChars() {
  // Takes an input from the Serial Moniter, then finds out 
  // what index position it is located at and creates a new
  // array with only the 3 characters required
  
  Serial.println("Please input the 3 characters you wish to display");
  Serial.println("They must be either: a letter from A-Z, the symbols \"!\" or \"?\", a digit from"); 
  Serial.println("0-9, or \"~\" for a whitespace");
  // Array in the same order as chars
  const char dictionary[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '?', '!', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '~'};
  int output[3]; // Array to hold index position of each character
  while (1) { // Forever loop, only breaks when 3 valid characters are inputed into the Serial moniter
    if (Serial.available() > 0) {
    Serial.println("Input recieved! Processing...");
    String inputChars; // String to hold Serial input
    inputChars = Serial.readString();
      if (inputChars.length() == 3) { // Makes sure only 3 characters were inputed
        // Two for loops, first one loops through each character
        // inputted, second loopss through every character in
        // dictionary. Finally, an if statement that checks if
        // the character inputted matches the one in the
        // dictionary, if so, append the index position to 
        // output
        for (int i = 0; i <= 2; i++) {
          // If the character is a letter, make it uppercase
          if (isAlpha(inputChars[i]) == true) {
          	inputChars[i] = inputChars[i] & ~(0x20);
          }
          for (int j = 0; j <= 38; j++) {
            if (dictionary[j] == inputChars[i]) {
              	output[i] = j;
              	break;
            }
          }
        }
        // If there was a character inputted that didn't match
        // any of the characters in dictionary, nothing would
        // have been appended to output, meaning either an 
        // invalid character has been appended or not enough
        // characters have been appended, this a quick check
        // to make sure that isn't the case and if it is, reset
        // output, and try again
        if (sizeof(output) != 6) {
          	Serial.println("Invalid string, each character must be a letter from A-Z;?;!;a number from 0-9.");
          	memset(output, 0, sizeof(output));
        } else {
		  // For every character, get the character that will be
          // used from chars, with the index positions in output
          // and append the 7 bytes to selectedChars
          memcpy(selectedChars[0], chars[output[0]], sizeof(chars[output[0]]));
          memcpy(selectedChars[1], chars[output[1]], sizeof(chars[output[1]]));
          memcpy(selectedChars[2], chars[output[2]], sizeof(chars[output[2]]));  
          Serial.println(inputChars);
          break;
          }
      } else {
    	Serial.println("Too many or few characters! You can only display 3 characters!");
   	  }
    }
  }
}

void setup() {
  // On switch
  while (1) {
    if (digitalRead(A3) == HIGH) {
    	break;
    }
  }
  
  pinMode(latchPinRow, OUTPUT);
  pinMode(dataPinRow, OUTPUT);  
  pinMode(clockPinRow, OUTPUT);
  
  // Pins controlling transistors cotrolling all column transistors
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  
  pinMode(latchPinColumn1, OUTPUT);
  pinMode(dataPinColumn1, OUTPUT);  
  pinMode(clockPinColumn1, OUTPUT);
  
  pinMode(latchPinColumn2, OUTPUT);
  pinMode(dataPinColumn2, OUTPUT);  
  pinMode(clockPinColumn2, OUTPUT);
  
  pinMode(latchPinColumn3, OUTPUT);
  pinMode(dataPinColumn3, OUTPUT);  
  pinMode(clockPinColumn3, OUTPUT);
  
  // Serial makes the program super slow, so I decided to take
  // the input in setup()
  Serial.begin(9600);
  getChars();
  Serial.end();
}

void loop() {
  displayChar(selectedChars[0], selectedChars[1], selectedChars[2]);
}
