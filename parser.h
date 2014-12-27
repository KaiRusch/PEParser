#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

typedef uint8 bool;
#define true 1
#define false 0

#define HEADERS_ONLY 0x1
#define SECTION 0x2

//File Header Struct
typedef struct
{
  uint16 magicNumber;
  uint16 sections;
  uint32 time;
  uint32 symbolOffset;
  uint32 numOfSymbols;
  uint16 optHeaderSize;
  uint16 flagBits;

}FileHeader;

//Optional Header struct
typedef struct
{
  uint32 magicNumber;
  uint8 majorLinker;
  uint8 minorLinker;
  uint32 codeSize;
  uint32 initializedData;
  uint32 uninitializedData;
  uint32 entryPoint;
  uint32 baseCode;
  uint32 baseData;
  uint32 baseImage;
  uint32 sectionAlignment;
  uint32 fileAlignment;
  uint16 majorOS;
  uint16 minorOS;
  uint16 majorImage;
  uint16 minorImage;
  uint16 majorSubsystem;
  uint16 minorSubsystem;
  uint32 win32Version;
  uint32 sizeImage;
  uint32 sizeHeaders;
  uint32 checksum;
  uint16 subsystem;
  uint16 dllChars;
  uint32 stackReserve;
  uint32 stackCommit;
  uint32 heapReserve;
  uint32 heapCommit;
  uint32 loaderFlags;
  uint32 numOfRVA;
}OptionalHeader;

//Section Header struct
typedef struct
{
  uint8 *sectionName;
  uint32 physicalAddress;
  uint32 virtualAddress;
  uint32 dataSize;
  uint32 dataAddress;
  uint32 relocationAddress;
  uint32 lineNumberAddress;
  uint16 relocEntries;
  uint16 lineNumberEntries;
  uint32 flagBits;
  
}Section;

//Symbol struct
typedef struct
{
  uint8* symbolName;
  uint32 symbolNumber;
  uint16 sectionNumber;
  uint16 symbolType;
  uint8 storageClass;
  uint8 auxilaryCount;
}Symbol;

void parse_file_header(uint8 *fileBuffer, int *pos,FileHeader *fileHeader);
void parse_opt_header(uint8 *fileBuffer, int pos, OptionalHeader *optionalHeader);
void parse_section(uint8 *fileBuffer, int *pos, Section *section);
void parse_symbol(uint8 *fileBuffer, int *pos, Symbol *symbol);

void print_file_header(FileHeader *fileHeader);
void print_optional_header(OptionalHeader *optionalHeader);
void print_section_info(Section *section);
void print_section_data(uint8 *fileBuffer, Section *section);
void print_symbol(Symbol *symbol);

#endif
