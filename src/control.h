#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "atl.h"
#include "evpath.h"

#include "internal.h"

class service;

namespace drift {

  class control {

    typedef struct {
      EVstone control_stone_;
      EVstone split_stone_;
      EVaction split_action_;
      EVsource source_;
      atom_t remote_stone_atom_;
      atom_t remote_contact_atom_;
    } control_info, *control_info_ptr;

  public:
    static int handle_heartbeat ( CManager cm, void *vevent, void *client_data, attr_list attrs );
    static void submit_heartbeat ( CManager cm, void *cdata );

    static int handle_advert ( CManager cm, void *vevent, void *cdata, attr_list attrs );
    static void submit_advert ( CManager cm, void *cdata );

  protected:

    control_info hc_, ac_;

    service *service_;
    
  public:
    control ( service& s );
    virtual ~control() {};
    
    private:
      control(const control&);
      control& operator=(const control&);
      
    };
    
}

#endif // __CONTROL_H__
