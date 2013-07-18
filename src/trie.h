#ifndef _TRIE_H_
#define _TRIE_H_

#include <list>
#include <string>
#include <memory>

#include <stdlib.h>
#include <unistd.h>

#include <boost/unordered_map.hpp>

using namespace std;

struct equal_char 
{
  bool operator()(char c1, char c2) const {
    return  (c1 == c2);
  }
};


template <class T>
class TrieNode
{
 public:

  //typedef __gnu_cxx::hash_map < char, TrieNode<T> *, __gnu_cxx::hash <char>, equal_char > trie_hash_t;
  typedef boost::unordered_map< char, TrieNode<T> * > trie_hash_t;

 public:

  TrieNode();
  TrieNode ( const char& ch, const T& data );
  virtual ~TrieNode();

  /* Remove all subtrees */
  virtual void clear();
  /* Remove all subtrees rooted at child c */
  virtual void clear ( const char& c );
  
  /* Fill and return a vector of the node values in my subtree */
  virtual void add_to_vector ( vector<T> &vec ) const;

  /* Return the number of nodes in my subtree, counting myself */
  virtual size_t size() const;

  /* Add a child node to my map indexed by ch */
  virtual void add_child ( const char& ch, const TrieNode<T>* child );

  /* Get the child node indexed by char c */
  virtual TrieNode<T>* get_child ( const char& c );

 public:

  char key_;
  T val_;

 protected:

  trie_hash_t children_;

};


template <class T>
class Trie
{
 public:
  
  Trie();
  Trie ( const string& key, const T& val );
  Trie ( const vector<string>& keys, const vector<T>& vals );
  virtual ~Trie();
  
  /*  Return the number of distinct strings indexed by this trie */
  virtual size_t size() const;

  /* 
   *  Put the value in the trie indexed by the key.  If the key doesn't 
   *  already exist, create it.
   */
  virtual void put ( const string& key, const T& val );

  /*
   *  Look for the key in the trie, returning true if found and the indexed
   *  value in val.  If not found, return false and val is unchanged.
   */
  virtual bool get ( const string& key, T& val ) const;

  /*
   *  Look for the longest prefix of key in the trie.  Return the 
   *  length of the prefix that was matched (return value of 0 indicates 
   *  no prefix of key matched, while return value of key.size() indicates
   *  the entire string matched).  If the return value is > 0, the matching
   *  value is in val, otherwise val is unchanged.
   */
  virtual size_t get_longest_prefix ( const string& key, T& val ) const;

  /*
   *  Get a pointer to the node stored at the longest prefix of key stored in the trie.
   *  Return value treated as in get_longest_prefix() above.  
   */
  virtual size_t get_longest_prefix_node ( const string& key, TrieNode<T>*& node ) const;

  /*
   *  Clear all entries.
   */
  virtual void clear();

  /*
   *  Clear all entries from the subtrie matching key.
   */
  virtual void clear ( const string& key );

  /*
   *  Return a pointer to the node at the key, NULL if this key is not present.
   */
  virtual TrieNode<T>* get_node ( const string& key ) const;

  /*
   *  Add my nodes to the given vector
   */
  virtual void add_to_vector ( vector<T>& vec ) const;

  /*
   *  Add all my nodes under the given prefix to the given vector
   */
  virtual void add_to_vector ( vector<T>& vec, const string& prefix ) const;

 protected:

  TrieNode<T> root_;

};

#endif /*  inclusion guard  */

  
  
