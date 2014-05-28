#include <algorithm>

#include "http_client.h"
#include "json.h"

#include "internal.h"
#include "part.h"

extern logging::sources::severity_logger<drift::severity_level> lg;


drift::immediate_part::~immediate_part()
{}


drift::immediate_part::immediate_part ( const bool now )
  : part ( now )
{}


drift::immediate_part::immediate_part ( const boost::uuids::uuid&, bool now ) 
  : part ( uuid, now )
{}


drift::immediate_part::immediate_part ( const unsigned long u )
  : immediate_.u_ ( u )
{}

drift::immediate_part::immediate_part ( const long l )
  : immediate_.l_ ( l )
{}

drift::immediate_part::immediate_part ( const double d )
  : immediate_.d_ ( d )
{}

drift::immediate_part::immediate_part ( const std::string& s )
  : immediate_.s_ ( s )
{}

void 
drift::immediate_part::json_props ( web::json::value& props )
{
  // get the superclass properties
  part::json_props ( props );

  // insert the immediate data properties by serializing the immediate JSON map
  props["immediate"] = immediate_.to_string();
}


void
drift::immediate_part::load()
{
  part::load();
}


void 
drift::immediate_part::store()
{
  part.store();
}

