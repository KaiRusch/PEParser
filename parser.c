//PE Parser © Kai Rusch, 2014

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
  printf("FILE HEADER:\n");
  
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

//Parses the optional header
void parse_opt_header(uint8 *fileBuffer, int pos)
{
  printf("OPTIONAL HEADER:\n");

  uint16 magicNumber = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint8 majorLinker = *((uint8 *)(fileBuffer + pos));
  pos += sizeof(uint8);
  uint8 minorLinker = *((uint8 *)(fileBuffer + pos));
  pos += sizeof(uint8);
  uint32 codeSize = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  uint32 initializedData = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  uint32 uninitializedData = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  uint32 entryPoint = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  uint32 baseCode = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32); 
  uint32 baseData = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  uint32 baseImage = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  uint32 sectionAlignment = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  uint32 fileAlignment = *((uint32 *)(fileBuffer + pos));
  pos += sizeof(uint32);
  uint16 majorOS = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint16 minorOS = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint16 majorImage = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint16 minorImage = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint16 majorSubsystem = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint16 minorSubsystem = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint32 win32Version = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint32 sizeImage = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint32 sizeHeaders = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint32 checksum = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint16 subsystem = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint16 dllChars = *((uint16 *)(fileBuffer + pos));
  pos += sizeof(uint16);
  uint32 stackReserve = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint32 stackCommit = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint32 heapReserve = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint32 heapCommit = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint32 loaderFlags = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);
  uint32 numOfRVA = *((uint32 *)(fileBuffer + pos));
  pos+= sizeof(uint32);


  printf("Magic Number: %X\nLinker Version: %d.%02d\nCode Size: %X\nSize of Initialized Data: %X\nSize of Unitialized Data: %X\nAddress of Entry Point: %X\nBase of Code: %X\nBase of Data: %X\nBase of Image: %X\nSection Alignment: %X\nFile Alignment: %X\nOperating System Version: %d.%02d\nImage Version: %d.%02d\nSubsystem Version: %d.%02d\nWin32 Version: %X\nSize of Image: %X\nSize of Headers: %X\nChecksum: %X\nSubsystem: %X\nDLL Characterisitics: %X\nSize of Stack Reserved: %X\nSize of Stack Committed: %X\nSize of Heap Reserved: %X\nSize of Heap Committed: %X\nLoader Flags: %X\nNumber of RVAs: %X\n\n"
	 ,magicNumber, majorLinker, minorLinker, codeSize, initializedData, uninitializedData,entryPoint,baseCode,baseData,baseImage,sectionAlignment,fileAlignment,majorOS,minorOS,majorImage,minorImage,majorSubsystem,minorSubsystem,win32Version,sizeImage,sizeHeaders,checksum,subsystem,dllChars,stackReserve,stackCommit,heapReserve,heapCommit,loaderFlags,numOfRVA);

}

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

//Parses a section
void parse_section(uint8 *fileBuffer, int *pos, Section *section)
{
  section->sectionName = (char *)(fileBuffer + *pos);
  *pos+=sizeof(char)*8;
  section->physicalAddress = *((uint32 *)(fileBuffer + *pos));
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

//Prints information about a section
void print_section_info(Section *section)
{
  printf("Section Name: ");
  for(int nameIndex = 0; nameIndex < 8; ++nameIndex)
    {
      printf("%c",*(section->sectionName + nameIndex));
    }
  printf("\n");

  printf("Physical Address: %X\nVirtual Address %X\nData Size: %X\nData Address: %X\nRelocation Address: %X\nLine Numbers Address: %X\nRelocation Entries: %X\nLine Number Entries: %X\nFlag: %X\n\n",
	 section->physicalAddress,section->virtualAddress,section->dataSize,section->dataAddress,section->relocationAddress,section->lineNumberAddress,section->relocEntries,section->lineNumberEntries,section->flagBits);

}

print_section_data(uint8 *fileBuffer, Section *section)
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

   printf("0x%08X    ", lineNumber);

   for(uint32 i = 0; i < section->dataSize; ++i)
     {
       printf("%c ",*(fileBuffer + section->dataAddress + i));
       if((i+1)%bytesPerLine == 0)
	 {
	   lineNumber += bytesPerLine;
	   printf("\n0x%08X    ",lineNumber);
	 }
     }
   printf("\n\n");

}

int main(int argc, char ** argv)
{
 
  char *filename = 0;

  uint8 options = 0;

  uint8* specifiedSection;

  //Parses command line arguments
  for(int i = 1; i < argc; ++i)
    {
      //Handles command line options
      if(argv[i][0] == '-')
	{
	  //Only display headers
	  if(argv[i][1] == 'H')
	    {
	     options = options | HEADERS_ONLY;
	    }
	  if(argv[i][1] == 'S')
	    {
	      options = options | SECTION;
	      ++i;
	      specifiedSection = argv[i];
	    }
	}
      else
	{
	  if(!filename)
	    {
	      filename = argv[i];
	    }
	  else
	    {
	      printf("Please only enter one filename\n");
	      return -1;
	    }
	}
    }
 
  //Check for valid input
  if(!filename)
    {
      printf("Please give an input file\n");
      return -1;
    }

  //Open the file
  uint8 *fileBuffer = open_file(filename);

  //Check that the buffer is not NULL
  if(!fileBuffer)
    {
      printf("Failed to open file\n");
      return -1;
    }

  //If the file is an executable
  bool executable = false;
  if(*fileBuffer == 'M' && *(fileBuffer+1) == 'Z')
    {
      executable = true;
    }

  //Checks if the file is a valid LIB
  if(!executable)
    {
      uint16 magicNumber = *((uint16 *)fileBuffer);
      if(magicNumber != 0x14c)
	{
	  printf("Not a valid file\n");
	  return -1;
	}
    }
 
  printf("Parsing %s\n\n",filename);

  //Position of the parser in the file
  int pos = 0;
  uint16 sections = 0;
  uint16 optHeaderSize = 0;

  if(executable)
    {
      pos = 0x3C;
      uint32 peAddress = *((uint32 *)(fileBuffer + pos));
      pos = peAddress;
      
      printf("Signature: ");

      for(int i = 0; i < 4; ++i)
	{
	  printf("%c",*((uint8 *)(fileBuffer + pos + i)));
	}
      printf("\n");
      pos+=4;

    }
  
  //Parse the header
  parse_header(fileBuffer,&pos, &sections, &optHeaderSize);
  
  //Parse the optional header if required
  if(optHeaderSize)
    {
      parse_opt_header(fileBuffer,pos);
    }

  pos+= optHeaderSize;

  Section *sectionPointer = (Section *)malloc(sections*sizeof(Section));
  
  //Parse the sections
  for(int sectionIndex = 0; sectionIndex< sections; ++sectionIndex)
    {
      uint32 dataSize;
      uint32 dataAddress;

      parse_section(fileBuffer, &pos, sectionPointer + sectionIndex);
      
      bool printSectionInfo = true;

      //If the -S option was used check if this section
      //was the specified section
      if(options & SECTION)
	{
	  bool matchingName = true;
	  uint8 *sectionName = (sectionPointer + sectionIndex)->sectionName;
	  for(uint8 nameIndex = 0; nameIndex < 8; ++nameIndex)
	    {
	      if(*(sectionName + nameIndex) != *(specifiedSection + nameIndex))
		{
		  matchingName = false;
		  break;
		}
	      if(*(sectionName + nameIndex) == 0 && *(specifiedSection + nameIndex) == 0)
		{
		  break;
		}
	    }
	  if(!matchingName)
	    {
	      printSectionInfo = false;
	    }
	}

      //Print section info if needed
      if(printSectionInfo)
	{
	  print_section_info(sectionPointer + sectionIndex);
	}

      if(printSectionInfo && !(options & HEADERS_ONLY))
	{
	  print_section_data(fileBuffer, sectionPointer + sectionIndex);
	}


    }
  


  return 0;
}


