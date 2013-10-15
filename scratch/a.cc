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
  http_client client("http://localhost:7474/db/data/");
 
  http_request req ( methods::GET );
  req.headers().add ( "Accept", "application/json" );
  // "  req.set_request_uri ( "requestpath" );
  return client.request(req).then([](http_response response) -> pplx::task<json::value>
				  {
				    if ( response.status_code() == status_codes::OK ) {
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
