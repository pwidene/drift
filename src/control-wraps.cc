#include <iostream>
#include <memory>

#include "formats.h"
#include "control.h"

using namespace std;

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {

  template<int handler_wrapper( void *msg, attr_list attrs )>
  static int
  action_handler_wrap ( CManager cm, void *msg, void *cdata, attr_list attrs )
  {
    shared_ptr<control> C (dynamic_cast<control*> cdata);
    return C->handler_wrapper( msg, attrs );
  }

  void
  control::action_setup ( EVstone st )
  {
    CManager cm = service_->cm();
    EVassoc_terminal_action ( cm, st, drift::put_i_immediate_formats,
			      control::action_handler_wrap<&control::put_immediate_action>,
			      this );
    EVassoc_terminal_action ( cm, st, drift::put_i_immediate_formats,
			      control::action_handler_wrap<&control::get_immediate_action>,
			      this );
    EVassoc_terminal_action ( cm, st, drift::simple_part_xfer_formats,
			      control::action_handler_wrap<&control::simple_part_xfer_action>,
			      this );
    EVassoc_terminal_action ( cm, st, drift::complex_part_xfer_formats,
			      control::action_handler_wrap<&control::complex_part_xfer_action>,
			      this );
  }

}

  
