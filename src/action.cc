#include <iostream>

#include "action.h"
#include "formats.h"
#include "service.h"

using namespace std;

extern logging::sources::severity_logger<drift::severity_level> lg;

EVstone drift::action::action_stone_;

void
drift::action::action_setup()
{
  CManager cm = drift::service::get_service()->cm();

  action_stone_ = EValloc_stone ( cm );

  // set up action handlers
  EVassoc_terminal_action ( cm, action_stone_, drift::put_i_immediate_formats,
			    drift::action::put_immediate_action, new drift::action::immed_type (immed_int) );
  EVassoc_terminal_action ( cm, action_stone_, drift::put_d_immediate_formats,
			    drift::action::put_immediate_action, new drift::action::immed_type (immed_double) );
  EVassoc_terminal_action ( cm, action_stone_, drift::put_s_immediate_formats,
			    drift::action::put_immediate_action, new drift::action::immed_type (immed_string) );
  EVassoc_terminal_action ( cm, action_stone_, drift::put_json_immediate_formats,
			    drift::action::put_immediate_action, new drift::action::immed_type (immed_json) );
  
  EVassoc_terminal_action ( cm, action_stone_, drift::simple_part_xfer_formats, drift::action::simple_part_xfer_action, NULL );
  
  EVassoc_terminal_action ( cm, action_stone_, drift::complex_part_xfer_formats, drift::action::complex_part_xfer_action, NULL );

}


int
drift::action::put_immediate_action ( CManager cm, void *msg, void *cdata, attr_list attrs )
{

  drift::action::immed_type *it = reinterpret_cast<drift::action::immed_type*> ( cdata );
  switch (it->type_) {
  case immed_int:
    break;
  case immed_double:
    break;
  case immed_string:
    break;
  case immed_json:
    break;
  }


  return 1;
}

int
drift::action::get_immediate_action ( CManager cm, void *msg, void *cdata, attr_list attrs )
{
  return 1;
}

int
drift::action::simple_part_xfer_action ( CManager cm, void *msg, void *cdata, attr_list attrs )
{
  return 1;
}

int
drift::action::complex_part_xfer_action ( CManager cm, void *msg, void *cdata, attr_list attrs )
{
  return 1;
}

