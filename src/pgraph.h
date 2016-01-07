#ifndef __PGRAPH_H__
#define __PGRAPH_H__

#include "boost/graph/adjacency_list.hpp"

using namespace std;

namespace drift {

  struct drift_props {
    string json;
    string attrs;
    drift_props() {};
    drift_props( const drift_props& dp ) = default;
  };


  typedef boost::adjacency_list<
    boost::listS,
    boost::listS,
    boost::directedS,
    drift_props,
    drift_props > PartGraph;


}

#endif /* inclusion guard */
