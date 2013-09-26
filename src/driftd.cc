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

//#include "mongo/client/dbclient.h"

#include "atl.h"
#include "evpath.h"

#include "internal.h"
#include "formats.h"
#include "control.h"

using namespace std;

namespace po = boost::program_options;

po::options_description driftd_opts("Allowed options");
logging::sources::logger lg;

int terminate_condition;
CManager myCM;

namespace {
  int shutdown = 0;
};


void 
usage(char* prog)
{
  cout << driftd_opts;
}



extern "C"
void 
close_handler( int signo ) 
{
  BOOST_LOG(lg) << "Shutting down.";
  CMCondition_signal ( myCM, terminate_condition );
}


int 
main (int argc , char *argv[]) 
{
  attr_list listen_info;
  
  struct sigaction new_action, old_action;
  int bootstrap_node = 0;

  /*
   *  Start logging services
   */
  boost::shared_ptr< logging::sinks::synchronous_sink< logging::sinks::text_ostream_backend > > sink =
    logging::add_console_log();

  sink->locked_backend()->auto_flush(true);
  logging::add_common_attributes();

  driftd_opts.add_options()
    ("init-network,n", po::value<bool>()->default_value(true), "Initialize network")
    ("bootstrap,b", po::value<bool>()->default_value(true), "Bootstrap network")
    ("contact-string,c", po::value<string>(), "Network contact string")
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
    
  /* 
   * Initialize the server network and setup all the message handlers 
   */
  myCM = CManager_create();

  terminate_condition = CMCondition_get ( myCM, NULL );

  /* Catch SIGINT, SIGTERM */  
  new_action.sa_handler = close_handler;
  sigemptyset(&new_action.sa_mask);
  
  sigaction(SIGINT,NULL,&old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction(SIGINT,&new_action,NULL);
  
  sigaction(SIGTERM,NULL,&old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction(SIGTERM,&new_action,NULL);
  
  drift::control::heartbeat_setup();

  /*
   *  Set up transport listener, fork comm thread, and wait to die
   *  control listener on port 44999 
   */  
  listen_info = create_attr_list();
  static atom_t CM_IP_PORT = attr_atom_from_string("IP_PORT");
  add_attr ( listen_info, CM_IP_PORT, Attr_Int4, reinterpret_cast<attr_value>(44999) );
  CMlisten_specific ( myCM, listen_info );

  BOOST_LOG(lg) << "Forking comm thread, ready to provide services.";
  CMfork_comm_thread (myCM);
  CMCondition_wait ( myCM, terminate_condition );
  
  return 0;
}
