
#include "http_client.h"
#include "json.h"

#include "internal.h"
#include "part.h"

extern logging::sources::severity_logger<drift::severity_level> lg;

drift::part::~part()
{}

drift::part::part()
{}


drift::part::part (const boost::uuids::uuid& tag, bool now) :
  tag_ (tag)
{
  if (now) {
    this->fill();
  }  
}


void
drift::part::fill()
{
  web::http::client::http_client cli("http://localhost:7474/db/data/");
  web::http::http_request req ( web::http::methods::GET );
  
  req.headers().add ( "Accept", "application/json" );







  
