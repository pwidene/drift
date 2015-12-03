#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "boost/geometry/index/rtree.hpp"
#include "boost/geometry/index/parameters.hpp"
#include "boost/geometry/geometries/point.hpp"
#include "boost/geometry/geometries/box.hpp"
#include "boost/geometry/geometries/adapted/boost_array.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/tuple/tuple.hpp"

#include "atl.h"
#include "evpath.h"

#include "internal.h"
#include "control.h"
#include "action.h"
#include "trie.h"
#include "part.h"

using namespace std;

namespace drift {

  class service {

  public:
    service();
    virtual ~service();

    static void close_handler ( int sig );
    
    CManager cm() const { return cm_; };
    void begin();
    
    char* service_endpoint() const;

    unique_ptr<control> c_;

    
    boost::dynamic_properties part_props, link_props;
    typedef boost::adjacency_list < listS, listS, directedS, part_props, link_props > PartGraph;
    PartGraph pgraph_;

  protected:
    
    CManager cm_;
    static int terminate_condition_;
    char *service_endpoint_;
    Trie<part*> master_index_;
  
  private:
    static service *instance_;
    service& operator=(const service&);
    service(const service&);
  };

}

    
#endif // __SERVICE_H__
