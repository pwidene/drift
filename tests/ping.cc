#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

extern "C" {
#include "evpath.h"
#include "atl.h"
}

#include "../src/formats.h"

using namespace std;
using namespace boost;

CManager cm;


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
    ("hostname,h", po::value<string>()->default_value("localhost"), "Server hostname")
    ("port,p", po::value<int>()->default_value(44999), "Server port")
    ;
  po::variables_map opts_vm;
  po::store( po::parse_command_line( argc, argv, ping_opts ), opts_vm );

  ifstream ifs("drift.cfg");
  if (ifs) {
    po::store( po::parse_config_file( ifs, ping_opts ), opts_vm );
    po::notify( opts_vm );
  }

  cm = CManager_create();

  attr_list contact_list = create_attr_list();
  static atom_t CM_HOSTNAME = attr_atom_from_string("IP_HOST");
  static atom_t CM_PORT = attr_atom_from_string("IP_PORT");
  add_attr ( contact_list, CM_HOSTNAME, Attr_String, (attr_value) opts_vm["hostname"].as<char*>() );
  add_attr ( contact_list, CM_PORT, Attr_Int4, (attr_value) opts_vm["port"].as<int>() );

  EVstone in_stone;
  in_stone = EVcreate_terminal_action( cm, drift::heartbeat_format_list, ping_handler, NULL );

  EVsource source_handle;
  int out_stone, remote_stone;
  out_stone = EValloc_stone( cm );
  EVassoc_bridge_action( cm, out_stone, contact_list, remote_stone );
  source_handle = EVcreate_submit_handle( cm, out_stone, drift::heartbeat_format_list );

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
