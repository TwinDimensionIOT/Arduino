#include <SoftWire.h>
#include <AsyncDelay.h>

int sdaPin = 44;
int sclPin = 46;

byte second = 0;
byte minute = 0;
byte hour = 0;
byte weekday = 0;
byte monthday = 0;
byte month = 0;
byte year = 0;

// I2C address of DS1307
const uint8_t I2C_ADDRESS = 0x68;

SoftWire sw(sdaPin, sclPin);
// These buffers must be at least as large as the largest read or write you perform.
char swTxBuffer[16];
char swRxBuffer[16];

AsyncDelay readInterval;


// Print with leading zero, as expected for time
void printTwoDigit(int n)
{
  if (n < 10) {
    Serial.print('0');
  }
  Serial.print(n);
}

void readTime(void)
{
  // Ensure register address is valid
  sw.enablePullups(true);
  sw.beginTransmission(I2C_ADDRESS);
  sw.write(uint8_t(0)); // Access the first register
  sw.endTransmission();

  uint8_t registers[7]; // There are 7 registers we need to read from to get the date and time.
  int numBytes = sw.requestFrom(I2C_ADDRESS, (uint8_t)7);
  for (int i = 0; i < numBytes; ++i) {
    registers[i] = sw.read();
  }
  if (numBytes != 7) {
    Serial.print("Read wrong number of bytes: ");
    Serial.println((int)numBytes);
    return;
  }

  int tenYear = (registers[6] & 0xf0) >> 4;
  int unitYear = registers[6] & 0x0f;
  int year = (10 * tenYear) + unitYear;

  int tenMonth = (registers[5] & 0x10) >> 4;
  int unitMonth = registers[5] & 0x0f;
  int month = (10 * tenMonth) + unitMonth;

  int tenDateOfMonth = (registers[4] & 0x30) >> 4;
  int unitDateOfMonth = registers[4] & 0x0f;
  int dateOfMonth = (10 * tenDateOfMonth) + unitDateOfMonth;

  // Reading the hour is messy. See the datasheet for register details!
  bool twelveHour = registers[2] & 0x40;
  bool pm = false;
  int unitHour;
  int tenHour;
  if (twelveHour) {
    pm = registers[2] & 0x20;
    tenHour = (registers[2] & 0x10) >> 4;
  } else {
    tenHour = (registers[2] & 0x30) >> 4;
  }
  unitHour = registers[2] & 0x0f;
  int hour = (10 * tenHour) + unitHour;
  if (twelveHour) {
    hour += 12;
  }

  int tenMinute = (registers[1] & 0xf0) >> 4;
  int unitMinute = registers[1] & 0x0f;
  int minute = (10 * tenMinute) + unitMinute;

  int tenSecond = (registers[0] & 0xf0) >> 4;
  int unitSecond = registers[0] & 0x0f;
  int second = (10 * tenSecond) + unitSecond;

  Serial.print("Date: ");
  Serial.print(year);
  Serial.print('-');
  printTwoDigit(month);
  Serial.print('-');
  printTwoDigit(dateOfMonth);
  Serial.print(" Time: ");
  printTwoDigit(hour);
  Serial.print(':');
  printTwoDigit(minute);
  Serial.print(':');
  printTwoDigit(second);
  Serial.println();
}

byte decToBcd(byte val) {
  return ((val/10*16) + (val%10));
}

byte readByte() {
  while (!Serial.available()) delay(10);
  byte reading = 0;
  byte incomingByte = Serial.read();
  while (incomingByte != '\n') {
    if (incomingByte >= '0' && incomingByte <= '9')
      reading = reading * 10 + (incomingByte - '0');
    else;
    incomingByte = Serial.read();
  }
  Serial.flush();
  return reading;
}

// This set of codes allows input of data
void setTime() {
  Serial.print("Please enter the current year, 00-99. - ");
  year = readByte();
  Serial.println(year);
  Serial.print("Please enter the current month, 1-12. - ");
  month = readByte();
  Serial.println(month);
  Serial.print("Please enter the current day of the month, 1-31. - ");
  monthday = readByte();
  Serial.println(monthday);
  Serial.println("Please enter the current day of the week, 1-7.");
  Serial.print("1 Sun | 2 Mon | 3 Tues | 4 Weds | 5 Thu | 6 Fri | 7 Sat - ");
  weekday = readByte();
  Serial.println(weekday);
  Serial.print("Please enter the current hour in 24hr format, 0-23. - ");
  hour = readByte();
  Serial.println(hour);
  Serial.print("Please enter the current minute, 0-59. - ");
  minute = readByte();
  Serial.println(minute);
  second = 0;
  Serial.println("The data has been entered.");

  // The following codes transmits the data to the RTC
  sw.beginTransmission(I2C_ADDRESS);
  sw.write(byte(0));
  sw.write(decToBcd(second));
  sw.write(decToBcd(minute));
  sw.write(decToBcd(hour));
  sw.write(decToBcd(weekday));
  sw.write(decToBcd(monthday));
  sw.write(decToBcd(month));
  sw.write(decToBcd(year));
  sw.write(byte(0));
  sw.endTransmission();
  // Ends transmission of data
}


void setup(void)
{
  Serial.begin(115200);

  while (!Serial) ; // Wait for Serial monitor to open
  delay(100);
  
  Serial.println("Write DS1307 example");
  Serial.print("    SDA pin: ");
  Serial.println(int(sdaPin));
  Serial.print("    SCL pin: ");
  Serial.println(int(sclPin));
  Serial.print("    I2C address: ");
  Serial.println(int(I2C_ADDRESS), HEX);

  sw.setTxBuffer(swTxBuffer, sizeof(swTxBuffer));
  sw.setRxBuffer(swRxBuffer, sizeof(swRxBuffer));
  sw.setDelay_us(5);
  sw.setTimeout(1000);
  sw.begin();

  setTime();
  
  readInterval.start(2000, AsyncDelay::MILLIS);
}


void loop(void)
{
  if (readInterval.isExpired()) {
    readTime();
    readInterval.restart();
  }
}
