#include <iostream>
#include <memory>

#include "formats.h"
#include "control.h"

using namespace std;

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {

#define ACTION_HELPER(theformats,handler) \
  EVassoc_terminal_action ( cm, st, drift::##theformats_formats, \
			    [](CManager cm, void* msg, void* cdata, attr_list a) \
			    {						\
			      shared_ptr<control> C (dynamic_cast<control*> ( cdata ) ); \
			      return C->##handler( msg, a ); \
			    },						\
			    this );					\

  
  void
  control::action_setup ( EVstone st )
  {
    CManager cm = service_->cm();

    ACTION_HELPER(put_i_immediate,put_immediate_action);
    ACTION_HELPER(put_i_immediate,get_immediate_action);
    ACTION_HELPER(simple_part_xfer,simple_part_xfer_action);
    ACTION_HELPER(complex_part_xfer,complex_part_xfer_action);

  }
}

  
