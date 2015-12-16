#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <memory>

#include "atl.h"
#include "evpath.h"

#include "internal.h"
#include "service.h"


namespace drift {

  class control {

  public:
    control ( service& s );
    virtual ~control() {};

    void add_hb_listener ( advert_ptr, attr_list );

    int handle_advert ( CManager cm, void *vevent, void *cdata, attr_list attrs );
    static void submit_advert ( CManager cm, void *cdata );

  protected:

    void action_setup( CManager, EVstone );

    template<typename T>
      void put_immediate_action( T msg, attr_list al )
      {
	service_.put_immediate( msg->val, msg->path, al );
      };

    
    /*
     *  The service is the source for:
     *    - heartbeat events. 
     *    -- need a source with the heartbeat format
     *    -- split stone
     *    -- bridge stone for each new subscriber contact, added to the split stone
     *
     *    - responses to control input (these are advertisements, in the "I just added
     *      something to index X" style). clients need to add a handler locally for the 
     *      advertisement event before sending a control message in order to properly
     *      get the ACK event (which they're presumably waiting for).
     *    -- need source with advertisement format
     *    -- split stone
     *    -- bridge stone for each new subscriber contact, added to the split stone
     *       (I'd expect that the split/bridge arrangements can be the same, since
     *        they're not type-specific?)
     *
     *  The service is the SINK for:
     *    - control input events (possibly multiple types)
     *    -- need handler for each type of input
     *    -- these map (eventually) to the handler stack which manipulates data
     *
     */
    EVstone keystone_;
    EVaction hb_split_action_;
    EVsource heartbeat_source_;

    EVstone request_stone_;
    EVsource advert_source_;

    atom_t remote_stone_atom_;
    atom_t remote_contact_atom_;
    

    service& service_;
        
    private:
      control(const control&);
      control& operator=(const control&);
      
    };
    
}

#endif // __CONTROL_H__
