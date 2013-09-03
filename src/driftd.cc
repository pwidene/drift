#include <unistd.h>
#include <signal.h>

#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#if Boost_MINOR_VERSION > 53
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#endif

#include <boost/program_options.hpp>

#include "gen_thread.h"
#include "evpath.h"

using namespace std;

#if Boost_MINOR_VERSION > 53
namespace logging = boost::log;
#endif
namespace po = boost::program_options;

po::options_description driftd_opts("Allowed options");

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
#if Boost_MINOR_VERSION > 53
  BOOST_LOG(lg) << "Shutting down.";
#endif
  CMCondition_signal ( myCM, terminate_condition );
}

extern "C"
void 
timer_handler( int signo ) 
{
#if Boost_MINOR_VERSION > 53
  BOOST_LOG(lg) << "Timer interrupt received.";
#endif
  //service.get_peer_load();
  //alarm(100);
  return;
}

int 
main (int argc , char *argv[]) 
{
  struct sigaction new_action, old_action;
  int bootstrap_node = 0;

  driftd_opts.add_options()
    ("init-network,n", po::value<bool>()->default_value(true), "Initialize network")
    ("bootstrap,b", po::value<bool>()->default_value(true), "Bootstrap network")
    ("contact-string,c", po::value<string>(), "Network contact string")
    ;
  po::variables_map opts_vm;
  po::store( po::parse_command_line( argc, argv, driftd_opts ), opts_vm );
  po::notify( opts_vm );
    
  /* 
   * Initialize the server network and setup all the message handlers 
   */
  //service.init_network();
  gen_pthread_init();
  myCM = CManager_create();

  terminate_condition = CMCondition_get ( myCM, NULL );

  new_action.sa_handler = close_handler;
  sigemptyset(&new_action.sa_mask);
  
  sigaction(SIGINT,NULL,&old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction(SIGINT,&new_action,NULL);
  
  sigaction(SIGTERM,NULL,&old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction(SIGTERM,&new_action,NULL);
  
  if (opts_vm["bootstrap"].as<bool>()) {
    struct sigaction old_timer, new_timer;
    
    new_timer.sa_handler = timer_handler;
    sigemptyset(&new_timer.sa_mask);
    sigaction(SIGALRM, NULL, &old_timer);
    if(old_timer.sa_handler != SIG_IGN)
      sigaction(SIGALRM, &new_timer, NULL);
  }

#if Boost_MINOR_VERSION > 53  
  BOOST_LOG(lg) << "Forking comm thread, ready to provide services.";
#endif

  CMlisten(myCM);
  CMfork_comm_thread (myCM);
  CMCondition_wait ( myCM, terminate_condition );
  
  return 0;
}

