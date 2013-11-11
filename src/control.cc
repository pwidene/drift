#include <iostream>

#include "control.h"
#include "formats.h"
#include "service.h"

using namespace std;

extern logging::sources::severity_logger<drift::severity_level> lg;

void
drift::control::heartbeat_setup()
{
  CManager myCM = drift::service::get_service()->cm();
  heartbeat_control_info *hci = reinterpret_cast<heartbeat_control_info*>(calloc(1, sizeof(heartbeat_control_info)));

  hci->control_stone = EValloc_stone(myCM);
  hci->split_stone = EValloc_stone(myCM);
  hci->split_action = EVassoc_split_action ( myCM, hci->split_stone, NULL );
  hci->remote_stone_atom = attr_atom_from_string("D_STONE");
  hci->remote_contact_atom = attr_atom_from_string("D_CONTACT_LIST");
  hci->source = EVcreate_submit_handle ( myCM, hci->split_stone, drift::heartbeat_formats );

  EVassoc_terminal_action ( myCM, 
			    hci->control_stone, 
			    drift::heartbeat_formats, 
			    drift::control::heartbeat_handler, 
			    hci );

  /* 
   *  CM periodic task for heartbeat
   */
  CMadd_periodic_task ( myCM, 2, 0, drift::control::submit_heartbeat, hci );
}

int
drift::control::heartbeat_handler ( CManager cm, void *vevent, void *client_data, attr_list attrs )
{
  EVstone remote_stone, new_bridge;
  char *clist_str;
  
  drift::heartbeat_ptr hbp = reinterpret_cast<drift::heartbeat_ptr>(vevent);
  heartbeat_control_info *hci = reinterpret_cast<heartbeat_control_info*>(client_data);
  
  get_int_attr ( attrs, hci->remote_stone_atom, &remote_stone );
  get_string_attr ( attrs, hci->remote_contact_atom, &clist_str );

  new_bridge = EValloc_stone( cm );
  EVassoc_bridge_action ( cm, new_bridge, attr_list_from_string(clist_str), remote_stone );
  EVaction_add_split_target ( cm, hci->split_stone, hci->split_action, new_bridge );
  return 1;
}


void 
drift::control::submit_heartbeat ( CManager cm, void *cdata )
{
  heartbeat_control_info *hci = reinterpret_cast<heartbeat_control_info*>(cdata);
  time_t ticks = boost::chrono::system_clock::to_time_t ( boost::chrono::system_clock::now() );

  drift::heartbeat hb;
  hb.ts = ticks;
  hb.flags = 0;

  EVsubmit ( hci->source, &hb, NULL );

  BOOST_LOG_SEV(lg, drift::debug) << "Heartbeat submission" << endl;

  return;
}
