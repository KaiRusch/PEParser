//PE Parser © Kai Rusch, 2014

#include <stdio.h>
#include <stdint.h>
#include <time.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

//Stores file data on the heap
//Returns pointer to data
uint8 *open_file(char *fileName)
{
  FILE *objFile = fopen(fileName,"rb");
  fseek(objFile,0L,SEEK_END);
  int fileSize = ftell(objFile);
  fseek(objFile,0L,SEEK_SET);
  uint8 *fileBuffer = (uint8 *)malloc(sizeof(uint8)*fileSize);
  fread(fileBuffer,sizeof(uint8),fileSize,objFile);
  fclose(objFile);

  return fileBuffer;
}

//Parses the first header of the OBJ file
void parse_header(uint8 *fileBuffer, int *pos, uint16 *sections, uint16* optHeaderSize)
{
  printf("File Header:\n");
  
  uint16 magicNumber = *((uint16 *)(fileBuffer + *pos));
  *pos += sizeof(uint16);
  *sections = *((uint16 *)(fileBuffer + *pos));
  *pos += sizeof(uint16);
  uint32 time = *((uint32 *)(fileBuffer + *pos));;
  *pos += sizeof(uint32);
  uint32 symbolOffset = *((uint32 *)(fileBuffer + *pos));
  *pos += sizeof(uint32);
  uint32 numOfSymbols = *((uint32 *)(fileBuffer + *pos));
  *pos += sizeof(uint32);
  *optHeaderSize = *((uint16 *)(fileBuffer + *pos));
  *pos += sizeof(uint16);
  uint16 flagBits = *((uint16 *)(fileBuffer + *pos));
  *pos += sizeof(uint16);

  time_t Time = (time_t)time;
  
  printf("Magic Number: %X\nSections: %X\nTime: %sAddress of Symbol Table: %X\nNumber of Symbols: %X\nOptional Header Size: %X\nFlags: %X\n\n", 
	 magicNumber, *sections, ctime(&Time),symbolOffset,
	 numOfSymbols,*optHeaderSize,flagBits);
}

//Parses a section
void parse_section(uint8 *fileBuffer, int *pos)
{
  char *sectionName = (char *)(fileBuffer + *pos);
  *pos+=sizeof(char)*8;

  uint32 physicalAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  uint32 virtualAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  uint32 dataSize = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  uint32 dataAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  uint32 relocationAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  uint32 lineNumberAddress = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);
  uint16 relocEntries = *((uint16 *)(fileBuffer + *pos));
  *pos+=sizeof(uint16);
  uint16 lineNumberEntries = *((uint16 *)(fileBuffer + *pos));
  *pos+=sizeof(uint16);
  uint32 flagBits = *((uint32 *)(fileBuffer + *pos));
  *pos+=sizeof(uint32);


  printf("Name of Section: ");
  for(int i = 0; i < 8; ++i)
    {
      printf("%c",*(sectionName + i));
    }
  printf("\nPhysical Address: %X\nVirtual Address: %X\nData Size: %X\nData Address: %X\nRelocation Table Address: %X\nLine Numbers Address: %X\nNumber of Relocations: %X\nNumber of Line Numbers: %X\nFlag Bits: %X\n\n",
	 physicalAddress,virtualAddress,dataSize,dataAddress,relocationAddress,lineNumberAddress,relocEntries,lineNumberEntries,flagBits);

  printf("Data:\n");

  for(int i = 0; i < dataSize; ++i)
    {
      printf("%02X ",*(fileBuffer + dataAddress + i));
      if((i+1)%10 == 0)
	{
	  printf("\n");
	}
    }

  printf("\n\nANSI Data:\n");


  for(int i = 0; i < dataSize; ++i)
    {
      printf("%c",*(fileBuffer + dataAddress + i));
      if((i+1)%30 == 0)
	{
	  printf("\n");
	}
    }
  printf("\n\n");
}

int main(int argc, char ** argv)
{

  //Check for valid input
  if(argc != 2)
    {
      printf("Only give one input file");
      return -1;
    }
  printf("Parsing %s\n\n",argv[1]);

  //Open the file
  uint8 *fileBuffer = open_file(argv[1]);

  //Check that the buffer is not NULL
  if(!fileBuffer)
    {
      printf("Failed to open file\n");
      return -1;
    }
 
  //Position of the parser in the file
  int pos = 0;

  uint16 sections = 0;
  uint16 optHeaderSize = 0;
  
  //Parse the header
  parse_header(fileBuffer,&pos, &sections, &optHeaderSize);
  
  //Parse the optional header if required
  if(optHeaderSize)
    {
      printf("Parse Opt Header here!\n");
      pos+=optHeaderSize; 
    }
  
  //Parse the sections
  for(int i = 0; i < sections; ++i)
    {
      parse_section(fileBuffer, &pos);
    }
  


  return 0;
}


