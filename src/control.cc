#include <iostream>

#include "control.h"
#include "formats.h"

using namespace std;

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {
  
  control::control ( service& s ) 
    : service_ (s)
  {
    CManager myCM = service_.cm();

    remote_stone_atom_ = attr_atom_from_string("drift:stone");
    remote_contact_atom_ = attr_atom_from_string("drift:client-contact-list");

    hb_stone_ = EValloc_stone( myCM );
    //    EVassoc_terminal_action( myCM, keystone_
    hb_split_action_ = EVassoc_split_action ( myCM, hb_stone_, NULL );
    heartbeat_source_ = EVcreate_submit_handle ( myCM, hb_stone_, heartbeat_formats );

    advert_source_ = EVcreate_submit_handle ( myCM, ad_stone_, advert_formats );
    advert_split_action_ = EVassoc_split_action ( myCM, ad_stone_, NULL );

    rq_stone_ = EValloc_stone( myCM );
    action_setup( myCM, rq_stone_ );

    /*
     * install action handler for heartbeat add request
     */
    EVassoc_terminal_action ( myCM, keystone_, advert_formats,
			      [](CManager cm, void *msg, void *cdata, attr_list al) {
				shared_ptr<control> C ( reinterpret_cast<control*>( cdata ) );
				shared_ptr<advert> msg ( reinterpret_cast<advert_ptr>( msg ) );
				C->add_heartbeat_listener( msg, al );
			      },
			      this );
				
    /* 
     *  CM periodic task for heartbeat
     */
    CMadd_periodic_task ( myCM, 2, 0,
			  [](CManager cm, void* cdata)
			  {
			    shared_ptr<control> C (reinterpret_cast<control*>(cdata));
			    time_t ticks =
			      boost::chrono::system_clock::to_time_t ( boost::chrono::system_clock::now() );
			    
			    drift::heartbeat hb;
			    hb.ts = ticks;
			    hb.flags = 0;

			    attr_list al;
			    
			    
			    EVsubmit ( C->heartbeat_source_, &hb, NULL );

			    // can i get away with this? extern scope of lg inside lambda?
			    BOOST_LOG_SEV(lg, drift::debug) << "Heartbeat submission" << endl;
			    
			    return;
			  },
			  this );


    //EVassoc_terminal_action ( myCM, stone_, drift::advert_formats, drift::handle_advert, this );

  }


  int
  control::add_heartbeat_listener ( advert_ptr msg, attr_list al )
  {
    EVstone remote_stone;
    char *clist_str;
  
    get_int_attr ( attrs, C->remote_stone_atom_, &remote_stone );
    get_string_attr ( attrs, C->remote_contact_atom_, &clist_str );

    new_bridge = EValloc_stone( cm );
    EVassoc_bridge_action ( cm, new_bridge, attr_list_from_string(clist_str), remote_stone );
    EVaction_add_split_target ( cm, C->hb_split_stone_, hb_split_action_, new_bridge );
    return 1;
  }


  int
  control::handle_advert ( CManager cm, void *vevent, void *cdata, attr_list attrs )
  {}


  void
  control::submit_advert ( CManager cm, void *cdata );
  {}


  
  void
  control::action_setup ( CManager myCM, EVstone st )
  {
    /*
     * I really wanted to do this with a templated function that would receive a pointer
     * to the callback. However, I couldn't figure out how to make that work when the
     * callback was going to be a class instance method. There's probably a way to make it
     * work with std::bind but this is just as kludgy and got me back to real work
     * faster.
     */

#define ACTION_HELPER(FMT,ACTION)						\
    EVassoc_terminal_action ( myCM, st, drift::##FMT##_formats,	\
			      [](CManager cm, void* msg, void* cdata, attr_list a) \
			      {						\
				shared_ptr<control> C (dynamic_cast<control*> ( cdata ) ); \
				FMT##_ptr p = reinterpret_cast<FMT##_ptr> ( msg ); \
				return C->##action( msg, a );		\
			      },					\
			      this );   

    ACTION_HELPER(put_i_immediate,put_i_immediate_action);
    ACTION_HELPER(put_i_immediate,get_i_immediate_action);
    ACTION_HELPER(simple_part_xfer,simple_part_xfer_action);
    ACTION_HELPER(complex_part_xfer,complex_part_xfer_action);

  }


  void
  control::put_i_immediate_action( put_i_immediate_ptr msg, attr_list al )
  {
    s.put_immediate( msg->val, msg->path, al );
  }

  void
  control::get_i_immediate_action
  
}
