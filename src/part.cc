#include <algorithm>

#include "json/json.h"

#include "internal.h"
#include "part.h"

using namespace std;

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {

  part::~part()
  {}

  part::part ( drift::PartGraph& pg,
	       redox::Redox& rdx,
	       const bool now ) :
    pgraph_( pg ),
    rdx_( rdx )
  {
    dirty_ = New;
  }


  part::part ( const boost::uuids::uuid& tag,
	       drift::PartGraph& pg,
	       redox::Redox& rdx,
	       const bool now) :
    pgraph_( pg ),
    rdx_( rdx ),
    tag_ (tag)
  {
    dirty_ = New;
  }

  void
  part::json_props ( Json::Value& props )
  {
    props["tag"] = boost::uuids::to_string( tag_ );
    props["name"] = name_;
  }


 

  void
  part::remove()
  {
    try {
      rdx_.command<string>( {"DEL", boost::uuids::to_string( tag_ ) + ":immediate" },
			    [](redox::Command<string>& c) {
			      if (!c.ok()) throw &c;
			    });

    }
    catch (redox::Command<string>* cex) {
      // again right now do nothing
    }
  }



  void 
  part::adopt ( part& child )
  {
    /*
     *  Add the edge to the part graph
     */
    auto edpair = add_edge ( v_, child.v_, pgraph_ );
    auto now = boost::chrono::system_clock::now();
    if ( edpair.second ) {
      // new edge addition, store any edge properties
      ;
    
    }

    // Force a store on one or both parts?
  
  }


  void
  part::abandon()
  {

  }

  void
  part::abandon ( part& child )
  {
    /*
     *  To be clear, here, what we're doing is severing the relationship between 
     *    me and this particular child part.  Neither a shallow nor deep deletion.
     */
  }


  immediate_part::immediate_part( drift::PartGraph& pg,
				  redox::Redox& rdx,
				  const bool now )
    : part( pg, rdx, now )
  {
    if (now) {
      this->store();
    }  
  }

  immediate_part::immediate_part( const boost::uuids::uuid& tag,
				  drift::PartGraph& pg,
				  redox::Redox& rdx,
				  const bool now )
    : part( tag, pg, rdx, now )
  {
    if (now) {
      this->store();
    }  
  }
  
  void
  immediate_part::store()
  {
  
    /*
     *  build a redis key prefixed by the UUID '<uuid>:immediate' to index the stored value
     *  issue the appropriate redis command
     *
     *  any properties or metadata get stored at '<uuid>:properties' and '<uuid>:metadata' 
     *  adopt/abandon methods manage the child parts
     *
     *  redis should handle any key existence issues for us
     */
 
    try {
      rdx_.command<string>( {"SET", boost::uuids::to_string( tag_ ) + ':' + "immediate", immediate_.asString() },
			    [](redox::Command<string>& c) {
			      if (!c.ok()) throw &c;
			    });

      mtime_ = boost::chrono::system_clock::now();
      std::ostringstream ostr;
      ostr << mtime_;
      rdx_.command<string>( { "SET", boost::uuids::to_string( tag_ ) + ':' + "mtime", ostr.str() },
			    [](redox::Command<string>& c) {
			      if (!c.ok()) throw &c;
			    });
      
    }
    catch (redox::Command<string>* cex) {
      // right now do nothing
    }

  }




  void
  immediate_part::load()
  {
    try {
      rdx_.command<string>( {"GET", boost::uuids::to_string( tag_ ) + ':' + "immediate" },
 			    [](redox::Command<string>& c) {
			      if (!c.ok()) throw &c;
			    });
      dirty_[Immediate] = 0;
    
      // assign c.reply() to the immediate_ member
      // throw should protect us against errors
      // what to do about state mask?

    }
    catch (redox::Command<string>* cex) {
      // again right now do nothing
    }
  }



  external_part::external_part( drift::PartGraph& pg,
				redox::Redox& rdx,
				const bool now )
    : part( pg, rdx, now )
  {
    if (now) {
      this->store();
    }  
  }

  external_part::external_part( drift::PartGraph& pg,
				redox::Redox& rdx,
				const Json::Value& storage_meta,
				const bool now )
    : part( pg, rdx, now )
  {
    if (now) {
      this->store();
    }  
  }

  external_part::external_part( const boost::uuids::uuid& tag,
				drift::PartGraph& pg,
				redox::Redox& rdx,
				const bool now )
    : part( tag, pg, rdx, now )
  {
    if (now) {
      this->store();
    }  
}

  void
  external_part::load()
  {}

  void
  external_part::store()
  {}
  
}









  
