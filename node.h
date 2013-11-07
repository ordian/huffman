#ifndef NODE_H_
#define NODE_H_

#include <cstddef> /* NULL */
#include <limits>
#include <vector>
using std::vector; 

typedef unsigned int Size_t;

int const MAX_SYMBOLS = 
  1 + std::numeric_limits<unsigned char>::max();

int const CHAR_BITS = 
  std::numeric_limits<unsigned char>::digits;



class Node
{
public:
  Size_t frequency;
  unsigned char  c;
  Node *left, *right;
  
  Node(Size_t f, unsigned char ch)
    : frequency(f)
    , c(ch)
    , left(NULL)
    , right(NULL)
  {}
  
  Node(Node *L = NULL, Node *R = NULL) 
    : frequency(L? L->frequency + R->frequency : 0)
    , c('\0')
    , left(L)
    , right(R) 
  {}
};


struct ByFrequency
{
  bool operator()(const Node *x, const Node *y) const 
  { 
    /* std::priority_queue is max heap */
    return x->frequency > y->frequency; 
  }
};


#endif /* NODE_H_ */
