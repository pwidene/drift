#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "boost/geometry/index/rtree.hpp"
#include "boost/geometry/index/parameters.hpp"
#include "boost/geometry/geometries/point.hpp"
#include "boost/geometry/geometries/box.hpp"
#include "boost/geometry/geometries/adapted/boost_array.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/tuple/tuple.hpp"

#include "redox.hpp"

#include "atl.h"
#include "evpath.h"

#include "internal.h"
#include "trie.h"

using namespace std;

namespace drift {

  class part;
  class control;

  struct service_params {
    string redis_host;
    unsigned short redis_port;
  };
  
  struct drift_props {
    string json;
    string attrs;
  };

  /*!
   The Drift service class. Represents a single service instance.
  */
  class service {

  public:
    service( service_params& );
    virtual ~service();

    static void close_handler ( int sig );
    
    CManager cm() const { return cm_; };
    void begin();
    
    char* service_endpoint() const;

    unique_ptr<control> c_;

    typedef boost::adjacency_list < boost::listS, boost::listS, boost::directedS, drift_props, drift_props > PartGraph;
    PartGraph pgraph_;

    /*
     *  actions interface
     */
    void put_immediate( long int, string&, attr_list );
    void put_immediate( double, string&, attr_list );
    void put_immediate( string&, string&, attr_list );

    void get_immediate( long int &, string &, attr_list );
    void get_immediate( double&, string&, attr_list );
    void get_immediate( string&, string&, attr_list );

    
  protected:
    
    CManager cm_;
    static int terminate_condition_;
    char *service_endpoint_;
    Trie<part*> master_index_;

    redox::Redox rdx_;

    service_params sp_;
    
  private:
    static service *instance_;
    service& operator=(const service&);
    service(const service&);
  };

}

    
#endif // __SERVICE_H__
