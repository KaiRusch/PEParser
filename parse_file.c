#include "parser.h"

//Parses the first header of the file
void parse_file_header(uint8 *fileBuffer, int *pos,FileHeader *fileHeader)
{
  fileHeader->magicNumber = *((uint16 *)(fileBuffer + *pos));
  *pos += sizeof(uint16);
  fileHeader->sections = *((uint16 *)(fileBuffer + *pos));
  *pos += sizeof(uint16);
  fileHeader->time = *((uint32 *)(fileBuffer + *pos));;
  *pos += sizeof(uint32);
  fileHeader->symbolOffset = *((uint32 *)(fileBuffer + *pos));
  *pos += sizeof(uint32);
  fileHeader->numOfSymbols = *((uint32 *)(fileBuffer + *pos));
  *pos += sizeof(uint32);
  fileHeader->optHeaderSize = *((uint16 *)(fileBuffer + *pos));
  *pos += sizeof(uint16);
  fileHeader->flagBits = *((uint16 *)(fileBuffer + *pos));
  *pos += sizeof(uint16);
}

//Parses the optional header
void parse_opt_header(uint8 *fileBuffer, int pos, OptionalHeader *optionalHeader)
{
  optionalHeader->magicNumber = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->majorLinker = *((uint8 *)(fileBuffer + pos));
  pos += sizeof(uint8);
  optionalHeader->minorLinker = *((uint8 *)(fileBuffer + pos));
  pos += sizeof(uint8);
  optionalHeader->codeSize = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  optionalHeader->initializedData = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  optionalHeader->uninitializedData = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  optionalHeader->entryPoint = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  optionalHeader->baseCode = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32); 
  optionalHeader->baseData = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  optionalHeader->baseImage = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  optionalHeader->sectionAlignment = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  optionalHeader->fileAlignment = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  optionalHeader->majorOS = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->minorOS = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->majorImage = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->minorImage = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->majorSubsystem = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->minorSubsystem = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->win32Version = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->sizeImage = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->sizeHeaders = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->checksum = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->subsystem = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->dllChars = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  optionalHeader->stackReserve = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->stackCommit = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->heapReserve = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->heapCommit = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->loaderFlags = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  optionalHeader->numOfRVA = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
}


//Parses a section
void parse_section(uint8 *fileBuffer, int *pos, Section *section)
{
  section->sectionName = (uint8 *)(fileBuffer + *pos);
  *pos+=sizeof(char)*8;
  section->virtualSize = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  section->virtualAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  section->dataSize = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  section->dataAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  section->relocationAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  section->lineNumberAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  section->relocEntries = *((uint16 *)(fileBuffer + *pos));
  *pos+=sizeof(uint16);
  section->lineNumberEntries = *((uint16 *)(fileBuffer + *pos));
  *pos+=sizeof(uint16);
  section->flagBits = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
}

//Parses a symbol
void parse_symbol(uint8 *fileBuffer, int *pos, Symbol *symbol)
{
  symbol->symbolName = (uint8*)(fileBuffer + *pos);
  *pos+=8*sizeof(uint8);
  symbol->symbolNumber = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  symbol->sectionNumber = *((uint16 *)(fileBuffer + *pos));
  *pos+=sizeof(uint16);
  symbol->symbolType = *((uint16 *)(fileBuffer + *pos));
  *pos+=sizeof(uint16);
  symbol->storageClass = *((uint8 *)(fileBuffer + *pos));
  *pos+=sizeof(uint8);
  symbol->auxilaryCount = *((uint8 *)(fileBuffer + *pos));
  *pos+=sizeof(uint8);
}
