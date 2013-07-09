/* Author: Swaroop Butala
      Implementation for KDtree. This is a wrapper around the kdtree index implmenetation in the libkdtree++_0.7.0 library
 */


#ifndef KDTREE_INDEX
#define KDTREE_INDEX

#include "baseindex.h"

#define KDTREE_DEFINE_OSTREAM_OPERATORS

// Make SURE all the asserts() are checked in the kdtree library
#undef NDEBUG

#include <kdtree++/kdtree.hpp>

#include <iostream>
#include <vector>
#include <limits>
#include <functional>
#include <set>

using namespace std;

// used to ensure all triplets that are accessed via the operator<< are initialised.
static set<const void*> registered;

struct triplet
{
  typedef float value_type;

  triplet(value_type a, value_type b, value_type c, const IndexDescPtr input)
  {
    	d[0] = a;
    	d[1] = b;
    	d[2] = c;
		
	
	if(input != NULL)
	{
		ind = new IndexDesc;
	    	ind->ID = input->ID;
	        ind->Status_Flag = input->Status_Flag;
	        ind->Source = input->Source;
	        ind->Client_Contact = input->Client_Contact;
	        ind->database_Object_Key = input->database_Object_Key;

	        if ( input->Links != NULL ) {
	                ind->Links = new LinkDesc;
	                list<string>::iterator it;
	                for ( it = input->Links->parent_links.begin(); \
	                        it != input->Links->parent_links.end(); it++)
	                        ind->Links->parent_links.push_back(*it);
		
        	        for ( it = input->Links->child_links.begin();  \
        	                it != input->Links->child_links.end(); it++)
        	                ind->Links->child_links.push_back(*it);
        	        ind->Links->root_link = input->Links->root_link;
        	}
        	else
        	        ind->Links = NULL;
	}else
	{
		ind = NULL;
	}
    	bool reg_ok = (registered.find(this) == registered.end());
    	assert(reg_ok);
    	registered.insert(this).second;
  }

  triplet(const triplet & x)
  {
    	d[0] = x.d[0];
    	d[1] = x.d[1];
    	d[2] = x.d[2];

	if(x.ind != NULL)
	{
		ind = new IndexDesc;
	
	        ind->ID = x.ind->ID;
	        ind->Status_Flag =  x.ind->Status_Flag;
	        ind->Source =  x.ind->Source;
	        ind->Client_Contact =  x.ind->Client_Contact;
	        ind->database_Object_Key =  x.ind->database_Object_Key;

	        if (  x.ind->Links != NULL ) {
	                ind->Links = new LinkDesc;
        	        list<string>::iterator it;
        	        for ( it =  x.ind->Links->parent_links.begin(); \
        	                it !=  x.ind->Links->parent_links.end(); it++)
        	                ind->Links->parent_links.push_back(*it);
	
        	        for ( it =  x.ind->Links->child_links.begin();  \
        	                it !=  x.ind->Links->child_links.end(); it++)
        	                ind->Links->child_links.push_back(*it);
        	        ind->Links->root_link =  x.ind->Links->root_link;
        	}
        	else
        	        ind->Links = NULL;
	}else
	{
		ind =NULL;
	}
    	bool reg_ok = (registered.find(this) == registered.end());
    	assert(reg_ok);
    	registered.insert(this).second;
  }

  ~triplet()
  {
    bool unreg_ok = (registered.find(this) != registered.end());
    assert(unreg_ok);
    registered.erase(this);
  }

  double distance_to(triplet const& x) const
  {
     double dist = 0;
     for (int i = 0; i != 3; ++i)
        dist += (d[i]-x.d[i])*(d[i]-x.d[i]);
     cout<<"calculated the distance = "<<sqrt(dist)<<endl;
     return sqrt(dist);
  }

  inline value_type operator[](size_t const N) const { return d[N]; }

  friend ostream& operator<<(std::ostream& out, triplet const& T);
  /*ostream& operator<<(std::ostream& out, triplet const& T)
  {
  	assert(registered.find(&T) != registered.end());
  	return out << '(' << T.d[0] << ',' << T.d[1] << ',' << T.d[2] << ')';
  }*/


  value_type d[3];
  IndexDescPtr ind;
};

inline bool operator==(triplet const& A, triplet const& B) {
  return A.d[0] == B.d[0] && A.d[1] == B.d[1] && A.d[2] == B.d[2];
}

inline ostream& operator<<(ostream& out, triplet const& T)
{
  assert(registered.find(&T) != registered.end());
  return out << '(' << T.d[0] << ',' << T.d[1] << ',' << T.d[2] << ')';
}

inline double tac( triplet t, size_t k ) { return t[k]; }

//Defining it for 3 dimensional space! Will have to be changed in case of other dimensions
typedef KDTree::KDTree<3, triplet, std::pointer_to_binary_function<triplet,size_t,double> > KD_tree;


//KDtree class

class KDtreeIndex : public  BaseIndex {

	KD_tree *indexKDTree;

public:
	KDtreeIndex(){}
	KDtreeIndex( string &dbname);
	void Add_Index( const IndexDesc &input){}
	void Add_Index (IndexDescPtr input);

	//These 2 get_indexes are useless and i dont care about them in kd trees
	IndexDescPtr  Get_Index (string &inp_id){}
        //overloading in case of the btrie based index
	IndexDescPtr  Get_Index (float inp_id){}

	IndexDescPtr Get_Index (float x, float y, float z);
	vector <IndexDescPtr> Get_Index(float x, float y, float z, float range);

	void Delete_Index(string &inp_id);
	~KDtreeIndex(){
		cout << "Index deleted ..." << endl;
		//TODO delete index
	}
};

#endif
	 
