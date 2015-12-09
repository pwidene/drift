
using namespace std;

#include <algorithm>


#include "json/json.h"

#include "internal.h"
#include "part.h"

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {

  part::~part()
  {}

  part::part ( const bool now ) :
    dirty_( New )
  {
  }


  part::part (const boost::uuids::uuid& tag, const bool now) :
    tag_ (tag), dirty_( New )
  {
    if (now) {
      this->store();
    }  
  }

void
part::json_props ( Json::Value& props )
{
  props["tag"] = tag_.to_string();
  props["name"] = name_;
}


void
part::store()
{
  
  

  //  An empty node_uri means we haven't been stored yet
  bool creating;
  if (node_uri_.empty()) {
    creating = true;
    req.set_method ( web::http::methods::POST );
  } else {
    creating = false;
    req.set_method ( web::http::methods::PUT );
  }

  req.headers().add ( "Accept", "application/json" );
  req.headers().add ( "Content-Type", "application/json" );

  web::json::value props;
  json_props ( props );
    
  req.set_body ( props );

  pplx::task<void> ptask = 
    client.request(req).then([](web::http::http_response response) -> pplx::task<json::value> {
	if ( response.status_code() == web::http::status_codes::Created and creating) {

	  // get the URI from the REST response and save it
	  web::http::http_headers::iterator i = response.find("Location");
	  if (i != response.end()) {
	    node_uri_ = i->second;
	  }

	  ctime_ = mtime_ = boost::chrono::system_clock::now();

	} else if ( response.status_code() == web::http::status_codes::NoContent and not creating ) {
	  
	  // this was a triumph, I'm making a note here, HUGE SUCCESS
	  mtime_ = boost::chrono::system_clock::now();

	} else {
	  
	  // We're in uncharted territory, pull the ripcord
	  throw response;
	}
      });
}



void
part::load()
{
  std::ostringstream ostr;
  ostr << part::get_n4j_rest_uri() << node_id_;
  web::http::client::http_client cli ( ostr.str() );
  web::http::http_request req ( web::http::methods::GET );
  
  req.headers().add ( "Accept", "application/json" );
  
  pplx::task<void> ptask = 
  client.request(req).then([](web::http::http_response response) -> pplx::task<json::value> {
      if ( response.status_code() == web::http::status_codes::OK ) {
	  return response.extract_json();
	}
	return pplx::task_from_result(json::value());
    });
    /*
      .then([](pplx::task<json::value> previousTask) {
	  try {
	    const json::value& v = previousTask.get();
	    for (auto iter = v.cbegin(); iter != v.cend(); ++iter) {
	      const json::value &str = iter->first;
	      const json::value &val = iter->second;
	      std::cout << "key: " << str.as_string() << ", value = " << val.to_string() << std::endl;
	    }
	  }
	  catch (const http_exception& e) {
	    std::ostringstream ss;
	    ss << e.what() << endl;
	    std::cout << ss.str();
	  }
	}
	);
    */
}


void
part::remove()
{
  std::ostringstream ostr;
  ostr << part::get_n4j_rest_uri() << "node/" << node_id_;
  web::http::client::http_client cli ( ostr.str() );
  web::http::http_request req ( web::http::methods::DELETE );

  req.headers().add ( "Accept", "application/json" );

  pplx::task<void> ptask = 
    client.request(req).then([](web::http::http_response response) -> pplx::task<json::value> {
	if ( response.status_code() == web::http::status_codes::Conflict ) {
	  throw response;
	}
	if ( response.status_code() == web::http::status_codes::NoContent ) {
	  return response.extract_json();
	}
	return pplx::task_from_result(json::value());
      });

  /*
    .then([](pplx::task<json::value> previousTask) {
	  try {
	    const json::value& v = previousTask.get();
	    for (auto iter = v.cbegin(); iter != v.cend(); ++iter) {
	      const json::value &str = iter->first;
	      const json::value &val = iter->second;
	      std::cout << "key: " << str.as_string() << ", value = " << val.to_string() << std::endl;
	    }
	  }
	  catch (const http_exception& e) {
	    std::ostringstream ss;
	    ss << e.what() << endl;
	    std::cout << ss.str();
	  }
	}
	);
  */
}



void 
part::adopt ( part& child )
{
  /*
   *  Add the edge to the part graph
   */
  auto edpair = add_edge ( v_, child.v_, pgraph_ );
  auto now = boost::chrono::system_clock::now();
  if ( edpair.second ) {
    // new edge addition, store any edge properties
    ;
    
  }

  // Force a store on one or both parts?
  
}


void
part::abandon()
{
  std::for_each ( parts_.begin(), parts_.end(), 
		  [](std::pair<std::string, part*> p) {
		      
		    web::http::client::http_client cli (p->second);
		    web::http::http_request req ( web::http::methods::DELETE );
		    req.headers().add ( "Accept", "application/json" );

		    pplx::task<void> ptask = 
		      client.request(req).then([](web::http::http_response response) -> pplx::task<json::value> {
			  if ( response.status_code() != web::http::status_codes::NoContent) {
			    throw response;
			  }
			  return pplx::task_from_result(json::value());
			});
		  })
}

void
part::abandon ( part& child )
{
  /* 
   *  To be clear, here, what we're doing is severing the relationship between 
   *    me and this particular child part.  Neither a shallow nor deep deletion.
   */

  web::http::client::http_client cli ( parts_[&child] ); 
  web::http::http_request req ( web::http::methods::DELETE );
  req.headers().add ( "Accept", "application/json" );

  pplx::task<void> ptask = 
    client.request(req).then([](web::http::http_response response) -> pplx::task<json::value> {
	if ( response.status_code() != web::http::status_codes::NoContent) {
	  throw response;
	}
	return pplx::task_from_result(json::value());
      });
}
      









  
