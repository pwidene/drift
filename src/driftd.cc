#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include <fstream>
#include <iostream>

#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/program_options.hpp"
#include "boost/log/utility/string_literal.hpp"
#include "boost/chrono/system_clocks.hpp"
#include "boost/scoped_ptr.hpp"

//#include "mongo/client/dbclient.h"

#include "service.h"

using namespace std;

namespace po = boost::program_options;

po::options_description driftd_opts("Allowed options");
logging::sources::severity_logger<drift::severity_level> lg(boost::log::keywords::severity = drift::debug);


void 
usage(char* prog)
{
  cout << driftd_opts;
}



int 
main (int argc , char *argv[]) 
{
  struct sigaction new_action, old_action;
  int bootstrap_node = 0;
  service_params sp;

  /*
   *  Start logging services
   */
  boost::shared_ptr< logging::sinks::synchronous_sink< logging::sinks::text_ostream_backend > > sink =
    logging::add_console_log();

  sink->locked_backend()->auto_flush(true);
  logging::add_common_attributes();

  
  /*
   *  Get the command-line and/or config file options
   */
  driftd_opts.add_options()
    ("help,h", "Display options help")
    ("redis-host,r", po::value<string>(&sp.redis_host)->default_value("localhost"), "Redis server hostname")
    ("redis-port", po::value<unsigned short>(&sp.redis_port)->default_value(6379), "Redis server port")
    ;
  po::variables_map opts_vm;
  try {
    po::store( po::parse_command_line( argc, argv, driftd_opts ), opts_vm );
  }
  catch( po::unknown_option& uo ) {
    cerr << argv[0] << ": " << uo.what() << endl;
  }
  catch ( exception& e ) {
    cerr << argv[0] << ": " << e.what() << endl;
  }
  catch (...) { cerr << argv[0] << ": unknown parsing error" << endl; }

  po::notify( opts_vm );
    
  /* Catch SIGINT, SIGTERM */  
  new_action.sa_handler = drift::service::close_handler;
  sigemptyset(&new_action.sa_mask);
  
  sigaction(SIGINT,NULL,&old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction(SIGINT,&new_action,NULL);
  
  sigaction(SIGTERM,NULL,&old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction(SIGTERM,&new_action,NULL);

  unique_ptr<drift::service> instance ( new drift::service( sp ) );
  instance->begin();

  return 0;
}
