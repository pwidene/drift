#include <iostream>

#include "control.h"
#include "formats.h"
#include "service.h"

using namespace std;

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {
  
  control::control ( drift::service& s ) 
    : service_ (s)
  {
    CManager myCM = service_->cm();

    stone_ = EValloc_stone( myCM );
    split_ = EVassoc_split_action ( myCM, stone_, NULL );
    remote_stone_atom_ = attr_atom_from_string("D_STONE");
    remote_contact_atom_ = attr_atom_from_string("D_CONTACT_LIST");

    heartbeat_source_ = EVcreate_submit_handle ( myCM, stone_, heartbeat_formats );
    advert_source_ = EVcreate_submit_handle ( myCM, stone_, advert_formats );

    action_setup( stone_ );
    
    /* 
     *  CM periodic task for heartbeat
     */
    CMadd_periodic_task ( myCM, 2, 0, drift::heartbeat_control::submit_heartbeat, this );

    EVassoc_terminal_action ( myCM, stone_, drift::advert_formats, drift::handle_advert, this );

  }


  int
  control::handle_heartbeat ( CManager cm, void *vevent, void *client_data, attr_list attrs )
  {
    EVstone remote_stone, new_bridge;
    char *clist_str;
  
    drift::heartbeat_ptr hbp = reinterpret_cast<drift::heartbeat_ptr>(vevent);
    heartbeat_control *ci = reinterpret_cast<heartbeat_control*>(client_data);
  
    get_int_attr ( attrs, ci->remote_stone_atom_, &remote_stone );
    get_string_attr ( attrs, ci->remote_contact_atom_, &clist_str );

    new_bridge = EValloc_stone( cm );
    EVassoc_bridge_action ( cm, new_bridge, attr_list_from_string(clist_str), remote_stone );
    EVaction_add_split_target ( cm, ci->split_stone_, ci->split_action_, new_bridge );
    return 1;
  }


  void 
  control::submit_heartbeat ( CManager cm, void *cdata )
  {
    heartbeat_control *ci = reinterpret_cast<heartbeat_control*>(cdata);
    time_t ticks = boost::chrono::system_clock::to_time_t ( boost::chrono::system_clock::now() );

    drift::heartbeat hb;
    hb.ts = ticks;
    hb.flags = 0;

    EVsubmit ( ci->source_, &hb, NULL );

    BOOST_LOG_SEV(lg, drift::debug) << "Heartbeat submission" << endl;

    return;
  }


  int
  control::handle_advert ( CManager cm, void *vevent, void *cdata, attr_list attrs )
  {}


  void
  control::submit_advert ( CManager cm, void *cdata );
  {}

}
