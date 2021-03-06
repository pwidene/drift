#include "boost/scoped_ptr.hpp"

#include "service.h"
#include "control.h"

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {
  
  service* service::instance_ = NULL;
  int service::terminate_condition_;

  service::~service() {}

  service::service( service_params& sp )
    : c_ (new control (*this)), sp_ (sp)
  {}


  char*
  service::service_endpoint() const
  {
    return service_endpoint_;
  }


  void
  service::begin()
  {
    attr_list listen_info;
    cm_ = CManager_create();

    /*
     *  Connect to the redis service
     */
    if( !rdx_.connect( sp_.redis_host, sp_.redis_port ) )
      throw "Cannot connect to Redis server, goodbye.";
    
    /*
     *  Listen for incoming service requests.  Get the contact information and save it
     *  to provide on the control/advertisement channel.  Do this before listening on the
     *  control port so the contact list is correct for advertising.
     */
    CMlisten ( cm_ );
    service_endpoint_ = attr_list_to_string ( CMget_contact_list ( cm_ ) );
  

    /*
     *  Set up transport listener, fork comm thread, and wait to die
     *  control listener on port 44999 
     */  
    listen_info = create_attr_list();
    static atom_t CM_IP_PORT = attr_atom_from_string("IP_PORT");
    add_attr ( listen_info, CM_IP_PORT, Attr_Int4, reinterpret_cast<attr_value>(44999) );
    CMlisten_specific ( cm_, listen_info );  

    BOOST_LOG_SEV(lg, drift::info) << "Forking comm thread, ready to provide services.";
    terminate_condition_ = CMCondition_get ( cm_, NULL );
    CMfork_comm_thread (cm_);
    CMCondition_wait ( cm_, terminate_condition_ );

    /* close Redis connection */
    rdx_.disconnect();
  }

  void 
  service::close_handler( int signo ) 
  {
    BOOST_LOG_SEV(lg, drift::info) << "Shutting down.";
    CMCondition_signal ( cm_, drift::service::terminate_condition_ );
  }

  void
  service::put_immediate( long int, string&, attr_list )
  {}
  
  void
  service::put_immediate( double, string&, attr_list )
  {}

  void
  service::put_immediate( string&, string&, attr_list )
  {}

  void
  service::get_immediate( long int &, string &, attr_list )
  {}
  
  void
  service::get_immediate( double&, string&, attr_list )
  {}

  void
  service::get_immediate( string&, string&, attr_list )
  {}

}
