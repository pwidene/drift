#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include <fstream>
#include <iostream>

#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"
#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/sinks/sync_frontend.hpp"
#include "boost/log/sinks/text_ostream_backend.hpp"
#include "boost/log/sources/logger.hpp"
#include "boost/log/sources/record_ostream.hpp"
#include "boost/log/utility/setup/console.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"

#include "boost/program_options.hpp"
#include "boost/log/utility/string_literal.hpp"
#include "boost/chrono/system_clocks.hpp"

#include "mongo/client/dbclient.h"

#include "atl.h"
#include "evpath.h"

#include "formats.h"

using namespace std;

typedef struct {
  EVstone control_stone;
  EVstone split_stone;
  EVaction split_action;
  EVsource source;
  atom_t remote_stone_atom;
  atom_t remote_contact_atom;
} heartbeat_control_info;


namespace logging = boost::log;
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

static int
heartbeat_handler ( CManager cm, void *vevent, void *client_data, attr_list attrs )
{
  EVstone remote_stone, new_bridge;
  char *clist_str;
  
  drift::heartbeat_ptr hbp = reinterpret_cast<drift::heartbeat_ptr>(vevent);
  heartbeat_control_info *hci = reinterpret_cast<heartbeat_control_info*>(client_data);
  
  get_int_attr ( attrs, hci->remote_stone_atom, &remote_stone );
  get_string_attr ( attrs, hci->remote_contact_atom, &clist_str );

  new_bridge = EValloc_stone(myCM);
  EVassoc_bridge_action ( myCM, new_bridge, attr_list_from_string(clist_str), remote_stone );
  EVaction_add_split_target ( myCM, hci->split_stone, hci->split_action, new_bridge );
  return 1;
}


extern "C"
void 
close_handler( int signo ) 
{
  BOOST_LOG(lg) << "Shutting down.";
  CMCondition_signal ( myCM, terminate_condition );
}

extern "C"
void 
submit_heartbeat ( CManager cm, void *cdata )
{

  heartbeat_control_info *hci = reinterpret_cast<heartbeat_control_info*>(cdata);
  time_t ticks = boost::chrono::system_clock::to_time_t ( boost::chrono::system_clock::now() );

  drift::heartbeat hb;
  hb.ts = ticks;
  hb.flags = 0;

  EVsubmit ( hci->source, &hb, NULL );

  BOOST_LOG(lg) << "Heartbeat submission" << endl;

  return;
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
  
  BOOST_LOG(lg) << "Forking comm thread, ready to provide services.";

  /*
   *  control listener on port 44999 
   */
  heartbeat_control_info *hci = reinterpret_cast<heartbeat_control_info*>(calloc(1, sizeof(heartbeat_control_info)));

  hci->control_stone = EValloc_stone(myCM);
  hci->split_stone = EValloc_stone(myCM);
  hci->split_action = EVassoc_split_action ( myCM, hci->split_stone, NULL );
  hci->remote_stone_atom = attr_atom_from_string("D_STONE");
  hci->remote_contact_atom = attr_atom_from_string("D_CONTACT_LIST");
  hci->source = EVcreate_submit_handle ( myCM, hci->split_stone, drift::heartbeat_format_list );

  EVassoc_terminal_action ( myCM, hci->control_stone, drift::heartbeat_format_list, heartbeat_handler, hci );

  /* 
   *  CM periodic task for heartbeat
   */
  CMadd_periodic_task ( myCM, 2, 0, submit_heartbeat, hci );

  /*
   *  Set up transport listener, fork comm thread, and wait to die
   */  
  listen_info = create_attr_list();
  static atom_t CM_IP_PORT = attr_atom_from_string("IP_PORT");
  add_attr ( listen_info, CM_IP_PORT, Attr_Int4, reinterpret_cast<attr_value>(44999) );
  CMlisten_specific ( myCM, listen_info );
  CMfork_comm_thread (myCM);
  CMCondition_wait ( myCM, terminate_condition );
  
  return 0;
}

