//PE Parser © Kai Rusch, 2014

#include "parser.h"

//Stores file data on the heap
//Returns pointer to data
uint8 *open_file(char *fileName)
{
  FILE *objFile = fopen(fileName,"rb");
  if(objFile == NULL)
    {
      return NULL;
    }
  fseek(objFile,0L,SEEK_END);
  int fileSize = ftell(objFile);
  fseek(objFile,0L,SEEK_SET);
  uint8 *fileBuffer = (uint8 *)malloc(sizeof(uint8)*fileSize);
  fread(fileBuffer,sizeof(uint8),fileSize,objFile);
  fclose(objFile);

  return fileBuffer;
}

bool matching_names(char *specifiedSection, char *sectionName)
{
  for(int nameIndex = 0; nameIndex < 8; ++nameIndex)
    {
      if(*(specifiedSection + nameIndex) != 
	 *(sectionName + nameIndex))
	{
	  return false;
	}
      if(*(specifiedSection + nameIndex) == '\0' &&
	 *(sectionName + nameIndex) == '\0')
	{
	  break;
	}
    }
  return true;
}

void parse_file(uint8 *fileBuffer, int *pos, FileHeader *fileHeader, OptionalHeader *optionalHeader, Section **sections, Symbol **symbols)
{
  parse_file_header(fileBuffer,pos,fileHeader);
  
  if(fileHeader->optHeaderSize)
    {
      parse_opt_header(fileBuffer,*pos,optionalHeader);
    }
  *pos += fileHeader->optHeaderSize;

  //Alocate memory for the section headers and symbols
  *sections = (Section *)malloc(fileHeader->sections*sizeof(Section));
  *symbols = (Symbol *)malloc(fileHeader->numOfSymbols*sizeof(Symbol));

  for(int sectionIndex = 0; sectionIndex < fileHeader->sections; ++sectionIndex)
    {
      Section *section = *sections + sectionIndex;
      parse_section(fileBuffer,pos,section);
    }

  for(int symbolIndex = 0; symbolIndex < fileHeader->numOfSymbols; ++symbolIndex)
    {
      Symbol *symbol = *symbols + symbolIndex;
      parse_symbol(fileBuffer,pos,symbol);
    }
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
	      if(i == argc - 1)
		{
		  printf("Please enter a section name after -S\n");
		}

	      else if(argv[i+1][0]=='-')
		{
		  printf("Please enter a section name after -S\n");
		}
	      else
		{
		  options = options | SECTION;
		  ++i;
		  specifiedSection = argv[i];
		}
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


  //Checks if the file is a valid OBJ
  if(!executable)
    {
      uint16 magicNumber = *((uint16 *)fileBuffer);
      if(magicNumber != 0x14c)
	{
	  printf("Not a valid file\n");
	  return -1;
	}
    }
 
  printf("\n\nPARSING %s\n\n",filename);

  if(executable)
    {
      printf("EXECUTABLE FILE:\n\n");
    }
  else
    {
      printf("COFF FILE:\n\n");
    }

  //Position of the parser in the file
  int pos = 0;

  if(executable)
    {
      pos = 0x3C;
      uint32 peAddress = *((uint32 *)(fileBuffer + pos));
      pos = peAddress;
      pos+=4;

    }

  FileHeader fileHeader;
  OptionalHeader optionalHeader;
  Section *sections = NULL;
  Symbol *symbols = NULL;

  parse_file(fileBuffer, &pos, &fileHeader,&optionalHeader, &sections, &symbols);

  if(!(options&SECTION))
    {
  
      print_file_header(&fileHeader);
      print_optional_header(&optionalHeader);
    }
  
  for(int sectionIndex = 0; sectionIndex < fileHeader.sections; ++sectionIndex)
    {
      Section *section = sections + sectionIndex;

      bool printSection = true;

      if(options & SECTION)
	{
	  
	  printSection = matching_names(specifiedSection,section->sectionName);
	}
      
      if(printSection)
	{
	  print_section_info(section);
	  if(!(options & HEADERS_ONLY))
	    {
	      print_section_data(fileBuffer,section);
	    }
	}
    }

  return 0;
}



