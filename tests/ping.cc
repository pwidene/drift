#include <iostream>

#include <boost/program_options.hpp>

#include "cercs_env.h"
#include "ffs.h"
#include "attr.h"
#include "gen_thread.h"
#include "evpath.h"

using namespace std;
using namespace boost;

CManager cm;

namespace drift {

  typedef struct _heartbeat_ptr {
    unsigned long ts;
    unsigned long flags;
  } heartbeat, *heartbeat_ptr;

  FMField heartbeat_field_list[] =
    {
      {"ts", "integer", sizeof(long), FMOffset(heartbeat_ptr, ts)},
      {"flags", "integer", sizeof(long), FMOffset(heartbeat_ptr, flags)},
      {NULL, NULL, 0, 0}
    };
}

static
int
ping_handler( CManager cm, void* vevent, void* cdata, attr_list attrs ) 
{
  drift::heartbeat_ptr event = static_cast< drift::heartbeat_ptr > ( vevent );

  cout << "Heartbeat received, timestamp " << event->ts;
  CManager_close( cm );
  return 0;
}


  
namespace po = boost::program_options;

po::options_description ping_opts("Allowed options");

int
main( int argc, char* argv[] )
{

  ping_opts.add_options()
    ("init-network,n", "Initialize network")
    ("bootstrap,b", "Bootstrap network")
    ("contact-string,c", po::value<string>(), "Network contact string")
    ;
  po::variables_map opts_vm;
  po::store( po::parse_command_line( argc, argv, ping_opts ), opts_vm );
  po::notify( opts_vm );

  gen_pthread_init();
  cm = CManager_create();

  attr_list contact_list;
  contact_list = create_attr_list();

  EVstone in_stone;
  in_stone = EValloc_stone( cm );
  EVassoc_terminal_action( cm, stone, ping_handler, NULL );

  EVsource source_handle;
  int out_stone, remote_stone;
  out_stone = EValloc_stone( cm );
  EVassoc_bridge_action( cm, stone, contact_list, remote_stone );
  source_handle = EVcreate_submit_handle( cm, out_stone, drift::heartbeat_field_list );

  /* TODO fix up contact list from prefs for server, generate mine for response */

  attr_list out_attrs = create_attr_list();
  drift::heartbeat hb;
  hb.ts = 0;
  hb.flags |= 16;
  EVsubmit( source_handle, &hb, out_attrs );
  CMrun_network( cm );
  
  free_attr_list( out_attrs );
  return 1;
}
