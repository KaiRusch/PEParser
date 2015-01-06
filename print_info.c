#include "parser.h"
#include <math.h>


void print_file_header(FileHeader *fileHeader)
{

  printf("FILE HEADER:\n\n");

  time_t convertedTime = (time_t)fileHeader->time;
  
  printf("Magic Number: %X\nSections: %X\nTime: %sAddress of Symbol Table: %X\nNumber of Symbols: %X\nOptional Header Size: %X\nFlags: %X\n\n", 
	 fileHeader->magicNumber, fileHeader->sections, ctime(&convertedTime),fileHeader->symbolOffset,
	 fileHeader->numOfSymbols,fileHeader->optHeaderSize,fileHeader->flagBits);
}

void print_optional_header(OptionalHeader *optionalHeader)
{
  printf("OPTIONAL HEADER:\n\nMagic Number: %X\nLinker Version: %d.%02d\nCode Size: %X\nSize of Initialized Data: %X\nSize of Unitialized Data: %X\nAddress of Entry Point: %X\nBase of Code: %X\nBase of Data: %X\nBase of Image: %X\nSection Alignment: %X\nFile Alignment: %X\nOperating System Version: %d.%02d\nImage Version: %d.%02d\nSubsystem Version: %d.%02d\nWin32 Version: %X\nSize of Image: %X\nSize of Headers: %X\nChecksum: %X\nSubsystem: %X\nDLL Characterisitics: %X\nSize of Stack Reserved: %X\nSize of Stack Committed: %X\nSize of Heap Reserved: %X\nSize of Heap Committed: %X\nLoader Flags: %X\nNumber of RVAs: %X\n\n"
	 ,optionalHeader->magicNumber, optionalHeader->majorLinker, optionalHeader->minorLinker, optionalHeader->codeSize, optionalHeader->initializedData, optionalHeader->uninitializedData,optionalHeader->entryPoint,optionalHeader->baseCode,optionalHeader->baseData,optionalHeader->baseImage,optionalHeader->sectionAlignment,optionalHeader->fileAlignment,optionalHeader->majorOS,optionalHeader->minorOS,optionalHeader->majorImage,optionalHeader->minorImage,optionalHeader->majorSubsystem,optionalHeader->minorSubsystem,optionalHeader->win32Version,optionalHeader->sizeImage,optionalHeader->sizeHeaders,optionalHeader->checksum,optionalHeader->subsystem,optionalHeader->dllChars,optionalHeader->stackReserve,optionalHeader->stackCommit,optionalHeader->heapReserve,optionalHeader->heapCommit,optionalHeader->loaderFlags,optionalHeader->numOfRVA);
}

//Prints information about a section
void print_section_info(Section *section)
{
  printf("Section Name: ");
  for(int nameIndex = 0; nameIndex < 8; ++nameIndex)
    {
      printf("%c",*(section->sectionName + nameIndex));
    }
  printf("\n");

  printf("VirtualSize: %X\nVirtual Address %X\nData Size: %X\nData Address: %X\nRelocation Address: %X\nLine Numbers Address: %X\nRelocation Entries: %X\nLine Number Entries: %X\nFlag: %X\n\n",
	 section->virtualSize,section->virtualAddress,section->dataSize,section->dataAddress,section->relocationAddress,section->lineNumberAddress,section->relocEntries,section->lineNumberEntries,section->flagBits);

}

void print_section_data(uint8 *fileBuffer, Section *section)
{

   printf("SECTION DATA:\n\n");

   uint32 lineNumber = 0;
   uint32 bytesPerLine = 16;

   printf("0x%08X    ", lineNumber);

   for(uint32 i = 0; i < section->dataSize; ++i)
     {
       printf("%02X ",*(fileBuffer + section->dataAddress + i));
       if((i+1)%bytesPerLine == 0)
	 {
	   lineNumber += bytesPerLine;
	   printf("\n0x%08X    ",lineNumber);
	 }
     }
   printf("\n\n");

   lineNumber = 0;
   bytesPerLine = 64;

   printf("0x%08X    ", lineNumber);

   for(uint32 i = 0; i < section->dataSize; ++i)
     {
       if(*(fileBuffer + section->dataAddress + i) < 32)
	 {
	   printf(".");
	 }
       else 
       {
	 printf("%c",*(fileBuffer + section->dataAddress + i));
       }
       if((i+1)%bytesPerLine == 0)
	 {
	   lineNumber += bytesPerLine;
	   printf("\n0x%08X    ",lineNumber);
	 }
     }
   printf("\n\n");

}


void print_symbol(Symbol *symbol)
{
  printf("Symbol Name: ");
  for(int nameIndex = 0; nameIndex < 8; ++nameIndex)
    {
      printf("%c",*(symbol->symbolName + nameIndex));
    }
  printf("\n");
}

