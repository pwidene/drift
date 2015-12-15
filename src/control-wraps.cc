#include <iostream>
#include <memory>

#include "formats.h"
#include "control.h"

using namespace std;

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {

  /*
   * I really wanted to do this with a templated function that would receive a pointer
   * to the callback. However, I couldn't figure out how to make that work when the
   * callback was going to be a class instance method. There's probably a way to make it
   * work with std::bind but this is just as kludgy and got me back to real work
   * faster.
   */

#define ACTION_HELPER(theformats,action)				 \
  EVassoc_terminal_action ( cm, st, drift::##theformats_formats, \
			    [](CManager cm, void* msg, void* cdata, attr_list a) \
			    {						\
			      shared_ptr<control> C (dynamic_cast<control*> ( cdata ) ); \
			      return C->##action( msg, a ); \
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

  
