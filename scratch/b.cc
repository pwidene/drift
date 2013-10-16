#include <sstream>
#include <iostream>

#include "http_client.h"
#include "json.h"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;

pplx::task<void> 
doit()
{
  http_client client("http://localhost:7474/db/data/node/");
 
  http_request req ( methods::POST );
  req.headers().add ( "Accept", "application/json" );
  req.headers().add ( "Content-Type", "application/json" );
  // "  req.set_request_uri ( "requestpath" );
  json::value obj;
  obj["name"] = json::value::string("Bob Loblaw");
  obj["age"] = json::value::number(55);
  
  req.set_body ( obj );

  return client.request(req).then([](http_response response) -> pplx::task<json::value>
				  {
				    if ( response.status_code() == status_codes::OK ||
					 response.status_code() == status_codes::Created) {
				      return response.extract_json();
				    }
				    return pplx::task_from_result(json::value());
				  }).then([](pplx::task<json::value> previousTask)
					  {
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

int main ( int argc, char *argv[] )
{
  doit().wait();
  return 1;
}
