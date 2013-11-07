#include "huffman.h"
#include <iostream>
#include <string>

Size_t filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::in | std::ifstream::binary);
    in.seekg(0, std::ifstream::end);
    return in.tellg(); 
}

static void usage()
{
  std::cerr << "Usage: [-c|-d] [-i <input file>] [-o <output file>]" 
	    << std::endl
	    << "-i - input file"
	    << std::endl
	    << "-o - output file"
	    << std::endl
	    << "-d - decompress\n"
	    << std::endl
	    << "-c - compress (default)\n"
	    << std::endl;
	
}

int main(int argc, char** argv)
{
  char compress = 1;
  const char *file_in = NULL, *file_out = NULL;
  std::ifstream  in;
  std::ofstream out;

  if (argc < 6) 
    {
      usage();
      return 1;
    }

  /* Get the command line arguments. */
  std::string i = "-i", o = "-o", c = "-c", d = "-d";
  for (int j = 1; j < argc; ++j)
    {
      std::string s(argv[j]);
      if (s == i)
	file_in  = argv[++j];
      
      else if (s == o)
	file_out = argv[++j];

      else if (s == c)
	compress = 1;

      else if (s == d)
	compress = 0;
	   
    }

  Size_t size = filesize(file_in);
  
  if (file_in)
    {
      in.open(file_in, std::ios::in | std::ios::binary);
      
      if (!in)
	{
	  std::cerr << "Can't open input file " 
		    << file_in 
		    << std::endl;
	  return 1;
	}
    }
  
  if (file_out)
    {
      out.open(file_out, std::ios::out | std::ios::binary);
      
      if (!out)
	{
	  std::cerr << "Can't open output file " 
		    << file_out 
		    << std::endl;
	  return 1;
	}
    }
  
  

  return compress ?
    huffmanEncodeFile(in, out, size) : huffmanDecodeFile(in, out);
}
