
#include "RIC3DFLASH.h"

static inline bool isTimedOut(uint32_t ts)
{
  return (long)(millis() - ts) >= 0;
}

// Read a line of input. Must be terminated with <CR> and optional <LF>
void readLine(char *line, size_t size)
{
  int c;
  size_t len = 0;
  uint32_t ts_waitLF = 0;
  bool seenCR = false;

  while (1) {
    c = Serial.read();
    if (c < 0) {
      if (seenCR && isTimedOut(ts_waitLF)) {
        // Line ended with just <CR>. That's OK too.
        goto end;
      }
      continue;
    }
    if (c != '\r') {
      // Echo the input, but skip <CR>
      Serial.print((char)c);
    }

    if (c == '\r') {
      seenCR = true;
      ts_waitLF = millis() + 100;       // Wait another .1 sec for an optional LF
    } else if (c == '\n') {
      goto end;
    } else {
      // Any other character is stored in the line buffer
      if (len < size - 1) {
        *line++ = c;
        len++;
      }
    }
  }
end:
  *line = '\0';
}

bool getUValue(const char *buffer, uint32_t * value)
{
  char *ptr;
  *value = strtoul(buffer, &ptr, 0);
  if (ptr != buffer) {
    return true;
  }
  return false;
}


/*
 * \brief Dump the contents of a data flash page
 */
void dumpPage(int page)
{
  if (page < 0)
    return;

  Serial.print("page "); Serial.println(page);
  dflash.readPageToBuf1(page);
  uint8_t buffer[16];
  for (uint16_t i = 0; i < DF_PAGE_SIZE; i += sizeof(buffer)) {
    size_t nr = sizeof(buffer);
    if ((i + nr) > DF_PAGE_SIZE) {
      nr = DF_PAGE_SIZE - i;
    }
    dflash.readStrBuf1(i, buffer, nr);

    dumpBuffer(buffer, nr);
  }
}

void dumpBuffer(uint8_t * buf, size_t size)
{
  while (size > 0) {
    size_t size1 = size >= 16 ? 16 : size;
    for (size_t j = 0; j < size1; j++) {
      // Silly Arduino Print has very limited formatting capabilities
      Serial.print((*buf >> 4) & 0xF, HEX);        // High nibble
      Serial.print(*buf & 0xF, HEX);               // Low nibble
      buf++;
    }
    Serial.println();
    size -= size1;
  }
}

// This is just an example that writes a few bytes
// to the flash page. Notice that the Buf1 isn't cleared
// nor filled with the previous contents of the flash page
void writePage(int page, String value)
{
  size_t stringsize = value.length();
  uint8_t buffer[stringsize];
  for (size_t i = 0; i < sizeof(buffer); ++i) {
    buffer[i] = value[i];
  }
  dflash.writeStrBuf1(0, buffer, sizeof(buffer));
  dflash.writeBuf1ToPage(page);
}