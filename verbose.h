// verbose.h
/* Debug mode */
/******************/
#define VERBOSE 1

#include <iostream>
#include <iomanip> /* setw() */
using std::setw;
using std::cout;
using std::endl;
/******************/

#include "node.h"

void printTree(Node * curr, int level) 
{
  if (VERBOSE)
  if (curr)
    {
      printTree(curr->right, level + 1);
      
      cout << setw(level * 5)
	   << curr->c 
	   << ": "
	   << curr->frequency
	   << ": "
	   << level
	   << endl;
      printTree(curr->left, level + 1);
    }
}

void printCodeLength(vector<Size_t> &length)
{
  if (VERBOSE)
    {
      cout << "Code length: " << endl;
      for (int c = 0; c < MAX_SYMBOLS; ++c)
	if (length[c])
	  cout << c 
	       << ": " 
	       << length[c]
	       << endl;
    }
} 

void printFrequences(vector<Size_t> &freq)
{
  if (VERBOSE)
    {
      cout << "Frequencies:" << endl;
      for (int c = 0; c < MAX_SYMBOLS; ++c)
	if (freq[c])
	  cout << c 
	       << ": " 
	       << freq[c]
	       << endl;
    }
}

void printStartCode(vector<Size_t> &startCode, Size_t max_length)
{
  if (VERBOSE)
    {
      cout << "Start code: " << endl;
      for (Size_t c = 1; c <= max_length; ++c)
	cout << c
	     << ": " 
	     << startCode[c]
	     << endl;
    }
}

void printReversedCode(vector<vector<bool> > &table)
{
  if (VERBOSE)
    {
      cout << "Reversed code: " << endl;
      for (int c = 0; c < MAX_SYMBOLS; ++c)
	if (table[c].size())
	  {
	    cout << (char) c
		   << ": ";
	    for (Size_t n = 0; n < table[c].size(); ++n)
	      cout << table[c][n];
	    cout << endl;
	  }    
    }
}
