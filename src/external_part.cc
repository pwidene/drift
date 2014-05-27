#include <algorithm>

#include "http_client.h"
#include "json.h"

#include "internal.h"
#include "part.h"

extern logging::sources::severity_logger<drift::severity_level> lg;

drift::external_part::~external_part()
{}

drift::external_part::external_part ( const bool now ) :
  part ( now )
{}

drift::external_part::external_part ( const web::json::value&  storage_meta ) :
  storage_meta_ ( storage_meta )
{}


drift::external_part::external_part ( const boost::uuids::uuid& uuid, bool now ) :
  part ( uuid, now )
{}


void
drift::external_part::json_props ( web::json::value& props )
{
  part::json_props ( props );

  std::ostringstream ostr;
  ostr << storage_meta_;
  props["storage_meta"] = ostr.str();
}


void
drift::external_part::load()
{
  part::load();
}



void
drift::external_part::store()
{
  part::store();
}

  
