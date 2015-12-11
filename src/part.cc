
using namespace std;

#include <algorithm>


#include "json/json.h"

#include "internal.h"
#include "part.h"

extern logging::sources::severity_logger<drift::severity_level> lg;

namespace drift {

  part::~part()
  {}

  part::part ( const bool now ) :
    dirty_( New )
  {
  }


  part::part (const boost::uuids::uuid& tag, const bool now) :
    tag_ (tag), dirty_( New )
  {
    if (now) {
      this->store();
    }  
  }

  void
  part::json_props ( Json::Value& props )
  {
    props["tag"] = tag_.to_string();
    props["name"] = name_;
  }


  void
  part::store()
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
      rdx_.command<string>( {"SET", tag_.to_string() + ':' + "immediate", immediate_.asString() },
			    [](redox::Command<string>& c) {
			      if (!c.ok()) throw c;
			    });

      std::ostringstream ostr;
      ostr << mtime_ = boost::chrono::system_clock::now();
      rdx_.command<string>( { "SET", tag_.to_string() + ':' + "mtime", ostr.str() },
			    [](redox::Command<string>& c) {
			      if (!c.ok()) throw c;
			    });
      
    }
    catch (redox::Command<string>& cex) {
      // right now do nothing
    }

}




void
part::load()
{
  try {
    rdx_.command<string>( {"GET", tag_.to_string() + ':' + "immediate" },
			  [](redox::Command<string>& c) {
			    if (!c.ok()) throw c;
			  });
    dirty_[Immediate] = 0;
    
    // assign c.reply() to the immediate_ member
    // throw should protect us against errors
    // what to do about state mask?

  }
  catch (redox::Command<string>& cex) {
    // again right now do nothing
  }
}


void
part::remove()
{
  try {
    rdx_.command<string>( {"DEL", tag_.to_string() + ":immediate" },
			  [](redox::Command<string>& c) {
			    if (!c.ok()) throw c;
			  });

  }
  catch (redox::Command<string>& cex) {
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
      









  
