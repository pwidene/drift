#include "trie.h"

template <class T>
drift::TrieNode<T>::TrieNode() :
  key_ ('\0'),
  val_ (NULL)
{
}


template <class T>
drift::TrieNode<T>::TrieNode ( const char& ch, const T& data ) :
  key_ (ch),
  val_ (data)
{
}


template <class T>
dritf::TrieNode<T>::~TrieNode()
{
}


template <class T>
void
drift::TrieNode<T>::clear()
{
  typename trie_hash_t::iterator n_iter;

  for (n_iter = children_.begin(); n_iter != children_.end(); n_iter++) {
    drift::TrieNode<T>* child = n_iter->second;
    child->clear();
    delete child;
  }
}


template <class T>
size_t
drift::TrieNode<T>::size() const
{
  size_t rval = 1; // count myself
  typename trie_hash_t::const_iterator hash_iter;

  for (hash_iter = children_.begin(); hash_iter != children_.end(); hash_iter++) {
    drift::TrieNode<T>* np = hash_iter->second;
    rval += np->size();
  }

  return rval;
}


template <class T>
void 
drift::TrieNode<T>::add_child ( const char& ch, const TrieNode<T>* child )
{
  children_.insert ( trie_hash_t::value_type ( ch, child ) );
}

template <class T>
drift::TrieNode<T>* 
drift::TrieNode<T>::get_child ( const char& ch )
{
  typename trie_hash_t::iterator i = children_.find ( ch );
  return (i != children_.end() ? i->second : NULL );
}


template <class T>
void
drift::TrieNode<T>::add_to_vector ( vector<T>& vec ) const
{
  typename trie_hash_t::iterator n_iter;

  vec.push_back ( val_ );
  for (n_iter = children_.begin(); n_iter != children_.end(); n_iter++) {
    drift::TrieNode<T>* child = n_iter->second;
    child->add_to_vector ( vec );
  }
}


/************************************************************
 *
 *  Trie class methods
 *
 ************************************************************/

template <class T>
Trie<T>::Trie()
{}

template <class T>
Trie<T>::Trie ( const string& key, const T& val )
{
  put ( key, val );
}

template <class T>
drift::Trie<T>::Trie ( const vector<string>& keys, const vector<T>& vals )
{
  vector<string>::const_iterator k_iter;
  vector<string>::const_iterator v_iter;

  for (k_iter = keys.begin(), v_iter = vals.begin();
       k_iter != keys.end() && v_iter != vals.end();
       k_iter++, v_iter++) {
    put ( *k_iter, *v_iter );
  }
}

template <class T>
drift::Trie<T>::~Trie()
{
  clear();
}


template <class T>
size_t 
drift::Trie<T>::size() const
{
  return root_.size();
}


template <class T>
void
drift::Trie<T>::put ( const string& key, const T& newval ) 
{
  string::const_iterator s_iter;

  drift::TrieNode<T>* new_child;
  drift::TrieNode<T>* node = root_;
  drift::TrieNode<T>* lookup = NULL;

  for (s_iter = key.begin(); s_iter != key.end(); s_iter++) {
    lookup = node->get_child ( *s_iter );
    if (!lookup) {
      /* no child node with that character.  Add one */
      node->add_child ( new_child = new drift::TrieNode<T> ( *s_iter, newval ) );
      node = new_child;
    } else {
      node = lookup;
    }
  }

  /* 
   *  We've gotten to the end of the key string, having followed 
   *  children or added them as necessary.  Store the value.
   */
  node->val_ = newval;
  return;
}


template <class T>
bool
drift::Trie<T>::get ( const string& key, T& val ) const
{
  T myval;
  size_t s = get_longest_prefix ( key, myval );

  if (s == key.size()) {
    val = myval;
    return true;
  }
  return false;
}


template <class T>
void
drift::Trie<T>::clear()
{
  root_.clear();
}


template <class T>
void
drift::Trie<T>::clear ( const string& key )
{
  get_node ( key ) -> clear();
}


template <class T>
drift::TrieNode<T>*
drift::Trie<T>::get_node ( const string& key ) const
{
  return NULL; 
}


template <class T>
void
drift::Trie<T>::add_to_vector ( vector<T>& vec ) const
{
  root_.add_to_vector ( vec );
}


template <class T>
void
drift::Trie<T>::add_to_vector ( vector<T>& vec, const string& prefix ) const
{
  drift::TrieNode<T>* node;

  if (get_longest_prefix_node ( prefix, node ) ) {
    node->add_to_vector ( vec );
  }

}


template <class T>
size_t
drift::Trie<T>::get_longest_prefix_node ( const string& key, drift::TrieNode<T>*& node ) const
{
  string::const_iterator s_iter;
  bool keep_going = true;
  drift::TrieNode<T>* search_node = root_;

  for (s_iter = key.begin(); keep_going && s_iter != key.end(); s_iter++) {
    keep_going = ( (search_node = search_node->get_child ( *s_iter )) != NULL );
  }

  if (s_iter != key.begin()) {
    node = search_node;
  }

  return s_iter - key.begin();
}


template <class T>
size_t
drift::Trie<T>::get_longest_prefix ( const string& key, T& val ) const
{
  size_t rval;
  drift::TrieNode<T>* node = NULL;
  
  rval = get_longest_prefix_node ( key, node );
  if (rval) {
    val = node->val_;
  }
  
  return rval;
}




  
