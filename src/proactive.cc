#include <string>
#include <cstddef>

#include "proactive.h"

#include "formats.h"

CManager drift::proactive::server_cm_ = static_cast<CManager> (0);

drift::proactive::proactive ( FMStructDescRec format_list[] )
  : stone_ ( static_cast<EVstone> ( -1 ) ),
    source_handle_ ( static_cast<EVsource> ( NULL ) ),
    format_list_ ( format_list )
{}

drift::proactive::~proactive()
{
  if ( source_handle_ != nullptr ) {
    EVfree_source ( source_handle_ );
  }
  if ( stone_ == -1 ) {
    EVfree_stone ( server_cm_, stone_ );
  }
}


void
drift::proactive::use_this_CM ( CManager cm )
{
  drift::proactive::server_cm_ = cm;
}

void 
drift::proactive::set_up_stone()
{
  if (stone_ == -1) {
    stone_ = EValloc_stone ( server_cm_ );
    split_ = EVassoc_split_action ( server_cm_, stone_, NULL );
  }

  if (source_handle_ == NULL) {
    source_handle_ = EVcreate_submit_handle ( server_cm_, stone_, format_list_ );
  }
}


EVstone
drift::proactive::add_target ( attr_list attrs, EVstone remote_stone )
{
  set_up_stone();
  EVstone s = EValloc_stone ( server_cm_ );
  EVassoc_bridge_action ( server_cm_, s, attrs, remote_stone );
  EVaction_add_split_target ( server_cm_, stone_, split_, s );
  return s;
}

void
drift::proactive::publish ( void *ev )
{
  EVsubmit ( source_handle_, ev, NULL );
}
