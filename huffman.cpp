#include <queue>
#include <algorithm> /* std::sort */
#include <fstream>
#include "node.h"
#include "verbose.h"

using std::priority_queue;


/* Helper functions */

void assignLength(Node             *root, 
                  vector<Size_t> &length,
                  Size_t           level)
{
  if (!root->left && !root->right)
    length[root->c] = level;

  if (root->left)
    assignLength(root->left,  length, level + 1);

  if (root->right)
    assignLength(root->right, length, level + 1);
}



void buildTable(Size_t                          max_length,
                vector<Size_t>                 &numberOfCodes,
                vector<Size_t>                 &startCode,
                vector<vector<unsigned char> > &buckets,
                vector<vector<bool> >          &table)
{
  Size_t counter = 0;
  /* Build code from start code */
  for (Size_t i = 1; i <= max_length; ++i)
    if (numberOfCodes[i])
      {
	counter = startCode[i];
	vector<unsigned char>::const_iterator p;
	for (p = buckets[i].begin(); p != buckets[i].end(); ++p)
	  {
	    /* Get first i bits in reversed order */
	    for (Size_t bit = 0; bit < i; ++bit)
	      table[*p].push_back(counter & (1 << bit));
	    
	    ++counter;
	  }
      }

}

void buildStartCode(Size_t          max_length, 
                    vector<Size_t> &numberOfCodes,
                    vector<Size_t> &startCode)
{
  Size_t code = 0;
  Size_t lastNumberOfCodes = 0;
  
  /* Find start code for each length. */
  for (Size_t count = max_length; count != 0; --count)
    {
      code += lastNumberOfCodes;
      code >>= 1;
      startCode[count] = code;
      lastNumberOfCodes = numberOfCodes[count];
    }
}

Node * buildEncodeTree(vector<Size_t> &freq)
{
  priority_queue<Node *, vector<Node *>, ByFrequency> t;
  for (int c = 0; c != MAX_SYMBOLS; ++c)
    if (freq[c])
      {  
	Node *p = new Node(freq[c], 
			   static_cast<unsigned char>(c));
	t.push(p);      
      }	
  
  while (t.size() > 1)
    {  
      Node *L = t.top();
      t.pop();
      
      Node *R = t.top(); 
      t.pop();
      
      Node *parent = new Node(L, R); 
      t.push(parent);
    }
  return t.top();
}

Node * buildDecodeTree(vector<unsigned char> &symb,
		       vector<vector<bool> > &table)
{
  Node *root = new Node;
  Node *cur = root;
  vector<unsigned char>::iterator sit;
  for (sit = symb.begin(); sit != symb.end(); ++sit)
    {
      Size_t size = table[*sit].size();
      cur = root;
      for (int bit = static_cast<int>(size) - 1; 
               bit != -1;
             --bit)
	{
	  if (table[*sit][bit]) /* go right */
	    {
	      if (!cur->right)
		{
		  Node *R = new Node;
		  cur->right = R;
		}
	      cur = cur->right;
	    }
	  else /* go left */
	    {
	      if (!cur->left)
		{
		  Node *L = new Node;
		  cur->left = L;
		}
	      cur = cur->left;
	    }
	}
      cur->c = (*sit); /* leaf */
    }
  return root;
}

void destroyTree(Node *root)
{
  if (root) {
    destroyTree(root->left);
    destroyTree(root->right);
    delete root;
  }
}

/* Format: max_length numberOfCodes[] chars_in_order */ 
void writeOverhead(Size_t                          max_length,
                   vector<Size_t>                 &numberOfCodes,
                   vector<vector<unsigned char> > &buckets,
                   std::ofstream                  &out)
{
  out << max_length;
  for (Size_t i = 1; i <= max_length; ++i)
    out << " " << numberOfCodes[i];
  out << " ";
  
  for (Size_t i = 1; i <= max_length; ++i)
    {
      vector<unsigned char>::const_iterator p;
      for (p = buckets[i].begin(); p != buckets[i].end(); ++p)
	out.put(*p);
    }
}

vector<vector<unsigned char> > 
readOverhead(std::ifstream &in,
             Size_t &max_length,
             vector<Size_t> &numberOfCodes,
             vector<unsigned char> &symb)
{
  in >> max_length; 
  for (Size_t i = 1; i <= max_length; ++i)
    in >> numberOfCodes[i];
    
  vector<vector<unsigned char> > buckets(max_length + 1,
					 vector<unsigned char>());

  in.get(); 
  for (Size_t l = 1; l <= max_length; ++l)
    for (Size_t i = 0; i != numberOfCodes[l]; ++i)
      { 
	unsigned char c;
	c = in.get();
	buckets[l].push_back(c);
	symb.push_back(c);
      }  
   return buckets;
}

void encode(std::ifstream &in,
            std::ofstream &out,
            vector<vector<bool> > &table)
{
  int count = 0; 
  unsigned char  buf = 0;
  in.clear(); 
  in.seekg(0);
  while (!in.eof())
    { 
      unsigned char c = in.get();
      vector<bool> &x = table[c];
      vector<bool>::reverse_iterator rit;
      for(rit = x.rbegin(); rit != x.rend(); ++rit)
	{
	  buf |=  (*rit) << (CHAR_BITS - 1 - count);   
	  ++count;   
	  if (count == CHAR_BITS) /* it's time to write */
	    { 
	      count = 0;   
	      out << buf;
	      buf = 0; 
	    } 
	}
    }
}

void decode(std::ifstream &in, std::ofstream &out, Node *root)
{
  Node *p = root;
  int count = 0; 
  unsigned char byte; 
  byte = in.get();
  
  while (!in.eof())
    {   
      bool b = byte & (1 << (CHAR_BITS - 1 - count)); 
      if (b) 
	p = p->right; 
      else 
	p = p->left;
      
      if (!p->left && !p->right) 
	{
	  if ((int) p->c != 255) /* EOF */
	    out.put(p->c); 
	  p = root;
	}
      ++count;
      if (count == CHAR_BITS)
	{
	  count = 0; 
	  byte = in.get();
	}
    }
  
}

void clean(Node *root)
{
  destroyTree(root);
}

/* End helper functions */

int huffmanEncodeFile(std::ifstream &in, std::ofstream &out)
{
  vector<Size_t> freq(MAX_SYMBOLS, 0);
  while (!in.eof())
    { 
      unsigned char c = in.get();
      ++freq[c];
    }
  printFrequences(freq);
  
  Node *root = buildEncodeTree(freq);
  printTree(root, 0);
 
  vector<Size_t> length(MAX_SYMBOLS, 0);
  assignLength(root, length, 0);
  printCodeLength(length);

 
  vector<Size_t> numberOfCodes(MAX_SYMBOLS, 0);  
  for (int c = 0; c != MAX_SYMBOLS; ++c)
    if (freq[c])
      ++numberOfCodes[length[c]];
   
  Size_t max_length = 0;
  for (int i = 0; i != MAX_SYMBOLS; ++i)
    if (numberOfCodes[i])
      max_length = i;
  
  vector<vector<unsigned char> > buckets(max_length + 1,
					 vector<unsigned char>());
  /* Sort chars by code length */
  for (int i = 0; i != MAX_SYMBOLS; ++i)
    if (freq[i])
      buckets[length[i]].push_back(static_cast<unsigned char>(i));
  
  /* Sort each bucket by ascii code */  
  for (Size_t i = 1; i <= max_length; ++i)
    std::sort(buckets[i].begin(), buckets[i].end());
  
  vector<Size_t> startCode(MAX_SYMBOLS, 0);
  buildStartCode(max_length, numberOfCodes, startCode);
  printStartCode(startCode, max_length); 

  vector<vector<bool> > table(MAX_SYMBOLS, vector<bool>());
  buildTable(max_length, 
	     numberOfCodes, 
	     startCode, 
	     buckets,
	     table);
  printReversedCode(table);

  writeOverhead(max_length, numberOfCodes, buckets, out);
  
  encode(in, out, table);
  
  clean(root);
  return 0;
}

int huffmanDecodeFile(std::ifstream &in, std::ofstream &out)
{
 
  vector<Size_t> numberOfCodes(MAX_SYMBOLS, 0);  
  Size_t max_length = 0; 
  
  vector<unsigned char> symb;
  vector<vector<unsigned char> > buckets;
  
  buckets = readOverhead(in, max_length, numberOfCodes, symb);
 
  vector<Size_t> startCode(MAX_SYMBOLS, 0); 
  buildStartCode(max_length, numberOfCodes, startCode);
  printStartCode(startCode, max_length);
  
  vector<vector<bool> > table(MAX_SYMBOLS, vector<bool>());  
  buildTable(max_length, 
	     numberOfCodes, 
	     startCode, 
	     buckets,
	     table);
  printReversedCode(table);

  Node *root = buildDecodeTree(symb, table);
  decode(in, out, root);
  clean(root);
  return 0;
}


