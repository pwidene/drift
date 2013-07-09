#include <unistd.h>
#include <signal.h>

#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/program_options.hpp>


using namespace std;

namespace logging = boost::log;
namespace po = boost::program_options;

po::options_description driftd_opts("Allowed options");

CMCondition terminate_condition;

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
  
  CMcondition_signal ( service.get_CM(), terminate_condition );
}

extern "C"
void 
timer_handler( int signo ) 
{
  BOOST_LOG(lg) << "Timer interrupt received.";
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
    ("init-network,n", "Initialize network")
    ("bootstrap,b", "Bootstrap network")
    ("contact-string,c", po::value<string>(), "Network contact string")
    ;
  po::variables_map opts_vm;
  po::store( po::parse_command_line( argc, argv, driftd_opts ), opts_vm );
  po::notify( opts_vm );
    
  /* 
   * Initialize the server network and setup all the message handlers 
   */
  service.init_network();

  terminate_condition = CMCondition_get ( service.get_CM(), NULL );

  new_action.sa_handler = close_handler;
  sigemptyset(&new_action.sa_mask);
  
  sigaction(SIGINT,NULL,&old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction(SIGINT,&new_action,NULL);
  
  sigaction(SIGTERM,NULL,&old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction(SIGTERM,&new_action,NULL);
  
  if (args_info.bootstrap_given) {
    struct sigaction old_timer, new_timer;
    
    new_timer.sa_handler = timer_handler;
    sigemptyset(&new_timer.sa_mask);
    sigaction(SIGALRM, NULL, &old_timer);
    if(old_timer.sa_handler != SIG_IGN)
      sigaction(SIGALRM, &new_timer, NULL);
  }
  
  BOOST_LOG(lg) << "Forking comm thread, ready to provide services.";
  CMfork_comm_thread (service.get_CM());
  CMCondition_wait ( service.get_CM(), terminate_condition );
  
  return 0;
}

