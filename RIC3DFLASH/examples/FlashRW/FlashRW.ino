
#include <RIC3DFLASH.h>

void getCommand();

//#########    setup        #############
void setup()
{
  Serial.begin(115200);

  dflash.init(SS);
}

void loop()
{
  getCommand();
}

// Read commands from Serial (the default Arduino serial port)
void getCommand()
{
  char line[100];
  char *ptr;
  uint32_t value;

  Serial.print("> ");
  readLine(line, sizeof(line));
  Serial.println();
  if (*line == '\0') {
  } else if (strcmp(line, "E Y") == 0) {
    Serial.print("DF chip erase ...");
    dflash.chipErase();
    Serial.println(" done");

  } else if (line[0] == 'D' || line[0] == 'd') {
    ptr = line + 1;
    if (getUValue(ptr, &value)) {
      int page = value;
      dumpPage(page);
    }

  } else if (line[0] == 'E' || line[0] == 'e') {
    ptr = line + 1;
    if (getUValue(ptr, &value)) {
      int page = value;
      dflash.pageErase(page);
    }

  } else if (line[0] == 'W' || line[0] == 'w') {
    ptr = line + 1;
    Serial.println("Enter the string to write to the page");
    while (Serial.available() == 0) {}
    String a = Serial.readString();
    if (getUValue(ptr, &value)) {
      int page = value;
      writePage(page, a);
    }
  }
}
