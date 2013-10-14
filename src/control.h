#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "atl.h"
#include "evpath.h"

#include "internal.h"

namespace drift {

  typedef struct {
    EVstone control_stone;
    EVstone split_stone;
    EVaction split_action;
    EVsource source;
    atom_t remote_stone_atom;
    atom_t remote_contact_atom;
  } heartbeat_control_info;
  
  class control {
  public:
    static int heartbeat_handler ( CManager cm, void *vevent, void *client_data, attr_list attrs );
    static void heartbeat_setup();
    static void submit_heartbeat ( CManager cm, void *cdata);
  private:
    control(const control&);
    control& operator=(const control&);

  };

}

#endif // __CONTROL_H__
