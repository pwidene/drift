
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
    this->store();
  }  
}

void
drift::part::json_props ( web::json::value& props )
{
  props["tag"] = json::value::string ( tag_.to_string() );
  props["name"] = json::value::string ( name_ );

  //needs to be a switch based on the union type being used
  props["immediate"] = json::value::number ( immediate_.i_ );
}


void
drift::part::store()
{
  std::ostringstream ostr;
  ostr << drift::part::get_n4j_rest_uri() << "node";
  web::http::client::http_client cli ( ostr.str() );
  web::http::http_request req;

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
drift::part::load()
{
  std::ostringstream ostr;
  ostr << drift::part::get_n4j_rest_uri() << node_id_;
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
drift::part::remove()
{
  std::ostringstream ostr;
  ostr << drift::part::get_n4j_rest_uri() << "node/" << node_id_;
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
drift::part::adopt ( part& child )
{
  std::ostringstream ostr;
  ostr << drift::part::get_n4j_rest_uri() << "node/" << node_id_;
  web::http::client::http_client cli ( ostr.str() );
  web::http::http_request req ( web::http::methods::POST );
  
  req.headers().add ( "Accept", "application/json" );
  req.headers().add ( "Content-Type", "application/json" );
  
  web::json::value body;
  body["to"] = drift::part::get_n4j_rest_uri() + "node/" + child.node_id_;
  body["type"] = "CHILD";
  req.set_body ( body );

  pplx::task<void> ptask = 
    client.request(req).then([](web::http::http_response response) -> pplx::task<json::value> {
	if ( response.status_code() == web::http::status_codes::Created) {
	  throw response;
	}
	if ( response.status_code() == web::http::status_codes::NoContent ) {
	  return response.extract_json();
	}
	return pplx::task_from_result(json::value());
      })  
    .then([](pplx::task<json::value> previousTask) {
	try {
	  const json::value& v = previousTask.get();
	  parts_.insert ( std::pair< v["Location"], &child > );
	}
	catch (const http_exception& e) {
	  std::ostringstream ss;
	  ss << e.what() << endl;
	  std::cout << ss.str();
	}
      }
      );
}


void
drift::part::abandon()
{
  for (auto iter = parts_.begin(); iter != parts_.end(); ++iter) {
    web::http::client::http_client cli (iter->second);
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
}

void
drift::part::abandon ( part& child )
{
  /* 
   *  To be clear, here, what we're doing is severing the relationship between 
   *    me and this particular child part.  Neither a shallow nor deep deletion.
   */

  // Linear search, is ugly, FIXME with better data structure?
  for (auto iter = parts_.begin(); iter != parts_.end(); ++iter) {
    if (iter->second == child) {
      web::http::client::http_client cli (iter->second);
      web::http::http_request req ( web::http::methods::DELETE );
      req.headers().add ( "Accept", "application/json" );

      pplx::task<void> ptask = 
	client.request(req).then([](web::http::http_response response) -> pplx::task<json::value> {
	    if ( response.status_code() != web::http::status_codes::NoContent) {
	      throw response;
	    }
	    return pplx::task_from_result(json::value());
	  });
      
      // assume we have only one possible child relationship with each child part, because
      // otherwise that would be weird
      break;
    }
  }
}








  
