
#include "service.h"

extern logging::sources::severity_logger<drift::severity_level> lg;

drift::service* drift::service::instance_ = NULL;
int drift::service::terminate_condition_;

drift::service::~service() {}

drift::service::service()
{}

drift::service*
drift::service::get_service()
{
  if (drift::service::instance_ == NULL) {
    drift::service::instance_ = new drift::service;
  }
  return drift::service::instance_;
}

CManager
drift::service::cm() const
{
  return cm_;
}

void
drift::service::begin()
{
  attr_list listen_info;
  cm_ = CManager_create();

  drift::control::heartbeat_setup();
  drift::action::action_setup();

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
}

void 
drift::service::close_handler( int signo ) 
{
  BOOST_LOG_SEV(lg, drift::info) << "Shutting down.";
  CMCondition_signal ( drift::service::get_service()->cm(), drift::service::terminate_condition_ );
}
