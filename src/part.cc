
#include "http_client.h"
#include "json.h"

#include "internal.h"
#include "part.h"

extern logging::sources::severity_logger<drift::severity_level> lg;

std::string n4j_rest_uri = "http://localhost:7474/db/data/";

const std::string&
drift::part::get_n4j_rest_uri() const
{
  return drift::part::n4j_rest_uri;
}


drift::part::~part()
{}

drift::part::part(const bool now)
{
  if (now)
    this->create();
}
  


drift::part::part (const boost::uuids::uuid& tag, bool now) :
  tag_ (tag)
{
  if (now) {
    this->load();
  }  
}

void
drift::part::create()
{
  std::ostringstream ostr;
  ostr << drift::part::get_n4j_rest_uri() << "node";
  web::http::client::http_client cli ( ostr.str() );
  web::http::http_request req ( web::http::methods::POST );
  
  req.headers().add ( "Accept", "application/json" );
  req.headers().add ( "Content-Type", "application/json" );

  web::json::value props;
  props["tag"] = json::value::string ( tag_.to_string() );
  props["name"] = json::value::string ( name_ );

  //needs to be a switch based on the union type being used
  props["immediate"] = json::value::number ( immediate_.i_ );
    
  req.set_body ( props );

  pplx::task<void> ptask = 
    client.request(req).then([](http_response response) -> pplx::task<json::value> {
	if ( response.status_code() == status_codes::OK ) {
	  return response.extract_json();
	}
	return pplx::task_from_result(json::value());
      }).then([](pplx::task<json::value> previousTask) {
	  try {
	    const json::value& v = previousTask.get();
	    for (auto iter = v.cbegin(); iter != v.cend(); ++iter) {
	      const json::value &str = iter->first;
	      const json::value &val = iter->second;
	      std::cout << "key: " << str.as_string() << ", value = " << val.to_string() << endl;
	    }
	  }
	  catch (const http_exception& e) {
	    ostringstream ss;
	    ss << e.what() << endl;
	    cout << ss.str();
	  }
	}
	);
}



void
drift::part::load()
{
  ostringstream ostr;
  ostr << drift::part::get_n4j_rest_uri() << node_id_;
  web::http::client::http_client cli ( ostr.str() );
  web::http::http_request req ( web::http::methods::GET );
  
  req.headers().add ( "Accept", "application/json" );
  
  pplx::task<void> ptask = 
    client.request(req).then([](http_response response) -> pplx::task<json::value> {
	if ( response.status_code() == status_codes::OK ) {
	  return response.extract_json();
	}
	return pplx::task_from_result(json::value());
      }).then([](pplx::task<json::value> previousTask) {
	  try {
	    const json::value& v = previousTask.get();
	    for (auto iter = v.cbegin(); iter != v.cend(); ++iter) {
	      const json::value &str = iter->first;
	      const json::value &val = iter->second;
	      std::cout << "key: " << str.as_string() << ", value = " << val.to_string() << endl;
	    }
	  }
	  catch (const http_exception& e) {
	    ostringstream ss;
	    ss << e.what() << endl;
	    cout << ss.str();
	  }
	}
	);
}

void
drift::part::store()
{
  ostringstream ostr;
  ostr << drift::part::get_n4j_rest_uri() << 







  
