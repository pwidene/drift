#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "boost/scoped_ptr.hpp"

#include "atl.h"
#include "evpath.h"

#include "internal.h"
#include "control.h"
#include "action.h"
#include "trie.h"
#include "part.h"


namespace drift {

  class service {

  public:
    service();
    virtual ~service();

    static void close_handler ( int sig );
    
    CManager cm() const { return cm_; };
    void begin();
    
    char* service_endpoint() const;

    boost::scoped_ptr<control> c_;

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
