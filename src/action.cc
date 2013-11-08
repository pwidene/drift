#include "control.h"
#include "formats.h"
#include "service.h"
#include "action.h"

extern logging::sources::severity_logger<drift::severity_level> lg;

EVstone drift::action::action_stone_;

void
drift::action::action_setup()
{
  CManager cm = drift::service::get_service()->cm();

  action_stone_ = EValloc_stone ( cm );

  // set up action handlers
  EVassoc_terminal_action ( cm, action_stone_, put_immediate_i_format_list, drift::action::put_immediate_action, new drift::action::immed_type (immed_int));
  EVassoc_terminal_action ( cm, action_stone_, put_immediate_d_format_list, drift::action::put_immediate_action, new drift::action::immed_type (immed_double));
  EVassoc_terminal_action ( cm, action_stone_, put_immediate_s_format_list, drift::action::put_immediate_action, new drift::action::immed_type (immed_string));
  EVassoc_terminal_action ( cm, action_stone_, put_immediate_json_format_list, drift::action::put_immediate_action, new drift::action::immed_type (immed_json) );

  EVassoc_terminal_action ( cm, action_stone_, simple_part_xfer_format_list, drift::action::simple_part_xfer_action, NULL );

  EVassoc_terminal_action ( cm, action_stone_, complex_part_xfer_format_list, drift::action::complex_part_xfer_action, NULL );

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

