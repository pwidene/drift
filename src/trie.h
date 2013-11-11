#ifndef _TRIE_H_
#define _TRIE_H_

#include <list>
#include <string>
#include <memory>

#include <stdlib.h>
#include <unistd.h>

#include "boost/unordered_map.hpp"

using namespace std;

struct equal_char 
{
  bool operator()(char c1, char c2) const {
    return  (c1 == c2);
  }
};


namespace drift {

  template <class T>
    class TrieNode
    {
    public:
      
      //typedef __gnu_cxx::hash_map < char, TrieNode<T> *, __gnu_cxx::hash <char>, equal_char > trie_hash_t;
      typedef boost::unordered_map< char, TrieNode<T> * > trie_hash_t;
      
    public:
      
    TrieNode() 
      : key_ ('\0'), val_ (NULL) 
	{};
      TrieNode ( const char& ch, const T& data ) : key_ (ch), val_ (data) {};
      ~TrieNode() {};
      
      /* Remove all subtrees */
      virtual void clear() 
      {
	typename trie_hash_t::iterator n_iter;
	
	for (n_iter = children_.begin(); n_iter != children_.end(); n_iter++) {
	  drift::TrieNode<T>* child = n_iter->second;
	  child->clear();
	  delete child;
	}

      };

      /* Remove all subtrees rooted at child c */
      virtual void clear ( const char& c ) {};
	
      
      /* Fill and return a vector of the node values in my subtree */
      virtual void add_to_vector ( vector<T> &vec ) const {
	typename trie_hash_t::const_iterator n_iter;
	
	vec.push_back ( val_ );
	for (n_iter = children_.begin(); n_iter != children_.end(); n_iter++) {
	  drift::TrieNode<T>* child = n_iter->second;
	  child->add_to_vector ( vec );
	}
      };
      
      /* Return the number of nodes in my subtree, counting myself */
      virtual size_t size() const {
	size_t rval = 1; // count myself
	typename trie_hash_t::const_iterator hash_iter;
	
	for (hash_iter = children_.begin(); hash_iter != children_.end(); hash_iter++) {
	  drift::TrieNode<T>* np = hash_iter->second;
	  rval += np->size();
	}     
	return rval;
      };

      
      /* Add a child node to my map indexed by ch */
      virtual void add_child ( const char& ch, TrieNode<T>* child ) {
	children_.insert ( typename trie_hash_t::value_type ( ch, child ) );
      };
      
      /* Get the child node indexed by char c */
      virtual TrieNode<T>* get_child ( const char& c ) const {
	typename trie_hash_t::const_iterator i = children_.find ( c );
	return (i != children_.end() ? i->second : NULL );
      };
      
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
  
      Trie() {};

      Trie ( const string& key, const T& val ) {
	put ( key, val );
      };

      Trie ( const vector<string>& keys, const vector<T>& vals ) {
	vector<string>::const_iterator k_iter;
	vector<string>::const_iterator v_iter;
	
	for (k_iter = keys.begin(), v_iter = vals.begin();
	     k_iter != keys.end() && v_iter != vals.end();
	     k_iter++, v_iter++) {
	  put ( *k_iter, *v_iter );
	}
      };


      ~Trie() {
	clear();
      };
      
      /*  Return the number of distinct strings indexed by this trie */
      virtual size_t size() const {
	return root_.size();
      };
      
      /* 
       *  Put the value in the trie indexed by the key.  If the key doesn't 
       *  already exist, create it.
       */
      virtual void put ( const string& key, const T& val ) {
	string::const_iterator s_iter;

	drift::TrieNode<T>* new_child;
	drift::TrieNode<T>* node = &root_;
	drift::TrieNode<T>* lookup = NULL;
	
	for (s_iter = key.begin(); s_iter != key.end(); s_iter++) {
	  lookup = node->get_child ( *s_iter );
	  if (!lookup) {
	    /* no child node with that character.  Add one */
	    node->add_child (*s_iter, new_child = new drift::TrieNode<T> ( *s_iter, val ) );
	    node = new_child;
	  } else {
	    node = lookup;
	  }
	}

	/* 
	 *  We've gotten to the end of the key string, having followed 
	 *  children or added them as necessary.  Store the value.
	 */
	node->val_ = val;
	return;
      };

      /*
       *  Look for the key in the trie, returning true if found and the indexed
       *  value in val.  If not found, return false and val is unchanged.
       */
      virtual bool get ( const string& key, T& val ) const {
	T myval;
	size_t s = get_longest_prefix ( key, myval );
	
	if (s == key.size()) {
	  val = myval;
	  return true;
	}
	return false;
      };

      /*
       *  Look for the longest prefix of key in the trie.  Return the 
       *  length of the prefix that was matched (return value of 0 indicates 
       *  no prefix of key matched, while return value of key.size() indicates
       *  the entire string matched).  If the return value is > 0, the matching
       *  value is in val, otherwise val is unchanged.
       */
      virtual size_t get_longest_prefix ( const string& key, T& val ) const {
	size_t rval;
	drift::TrieNode<T>* node = NULL;
	
	rval = get_longest_prefix_node ( key, node );
	if (rval) {
	  val = node->val_;
	}
	
	return rval;
      };

      /*
       *  Get a pointer to the node stored at the longest prefix of key stored in the trie.
       *  Return value treated as in get_longest_prefix() above.  
       */
      virtual size_t get_longest_prefix_node ( const string& key, TrieNode<T>*& node ) const {
	string::const_iterator s_iter;
	bool keep_going = true;
	const drift::TrieNode<T>* search_node = &root_;
	
	for (s_iter = key.begin(); keep_going && s_iter != key.end(); s_iter++) {
	  keep_going = ( (search_node = search_node->get_child ( *s_iter )) != NULL );
	}
	
	if (s_iter != key.begin()) {
	  node = const_cast< drift::TrieNode<T>* > (search_node);
	}
	
	return s_iter - key.begin();
      };

      /*
       *  Clear all entries.
       */
      virtual void clear() {
	root_.clear();
      };

      /*
       *  Clear all entries from the subtrie matching key.
       */
      virtual void clear ( const string& key ) {
	get_node ( key ) -> clear();
      };

      /*
       *  Return a pointer to the node at the key, NULL if this key is not present.
       */
      virtual TrieNode<T>* get_node ( const string& key ) const {
	return NULL;
      };

      /*
       *  Add my nodes to the given vector
       */
      virtual void add_to_vector ( vector<T>& vec ) const {
	root_.add_to_vector ( vec );
      };

      /*
       *  Add all my nodes under the given prefix to the given vector
       */
      virtual void add_to_vector ( vector<T>& vec, const string& prefix ) const {
	drift::TrieNode<T>* node;
	
	if (get_longest_prefix_node ( prefix, node ) ) {
	  node->add_to_vector ( vec );
	}
      };


    protected:

      TrieNode<T> root_;

    };

}
#endif /*  inclusion guard  */

  
  
