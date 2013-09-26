#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "atl.h"
#include "evpath.h"

#include "internal.h"
#include "control.h"

namespace drift {

  class service {

  public:
    service();
    virtual ~service();

    static service* get_service();
    static void close_handler ( int sig );
    
    CManager cm() const;
    void begin();

  protected:
    
    CManager cm_;
    static int terminate_condition_;
  
  private:
    static service *instance_;
    service& operator=(const service&);
    service(const service&);
  };

}

    
#endif // __SERVICE_H__
