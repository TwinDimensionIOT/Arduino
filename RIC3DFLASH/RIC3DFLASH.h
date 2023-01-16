#ifndef RIC3DFLASH_H
#define RIC3DFLASH_H

#include <Arduino.h>
#include <SPI.h>
#include <Sodaq_dataflash.h>

//######### forward declare #############
static inline bool isTimedOut(uint32_t ts);
void readLine(char *line, size_t size);
void getCommand();
bool getUValue(const char *buffer, uint32_t * value);
void dumpPage(int page);
void dumpBuffer(uint8_t * buf, size_t size);
void writePage(int page, String value);

#endif