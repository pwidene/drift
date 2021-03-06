/* -*- mode: c++; -*- */
#ifndef __PART_H__
#define __PART_H__

#include <string>
#include <unordered_map>
#include <bitset>

#include "json/json.h"
#include "redox.hpp"

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "boost/chrono.hpp"
#include "boost/tuple/tuple.hpp"



#include "internal.h"
#include "pgraph.h"

#include "proactive.h"

namespace drift {

  class part : public proactive {

  public:

    part ( drift::PartGraph&,
	   redox::Redox&,
	   const bool now = false );
    part ( const boost::uuids::uuid&,
	   drift::PartGraph&,
	   redox::Redox&,
	   bool now = false );
    virtual ~part();

    virtual void adopt( part& );
    virtual void abandon ( part& );
    virtual void abandon();
    virtual void remove();

    virtual void load() = 0;
    virtual void store() = 0;

  public:

    std::unordered_map < part*, std::string > parts_;
    std::string name_;

    // My UUID
    boost::uuids::uuid tag_;
    
    // Pointer to my vertex in the service's part graph
    drift::PartGraph::vertex_descriptor v_;

    // Creation and modification times
    boost::chrono::system_clock::time_point ctime_, mtime_;

  protected:

    virtual void json_props ( Json::Value& );

    std::bitset<8> dirty_;
    const short int Properties = 0;
    const short int External = 1;
    const short int Immediate = 2;
    const short int New = 7;

    // Reference to the whole part graph owned by the service instance
    // who created me
    drift::PartGraph& pgraph_;

    // Reference to the Redox server owned by the service instance who
    // created me
    redox::Redox& rdx_;

  private:
    
    part& operator= ( const part& );
    part ( const part& );

  };

  
  class external_part : public part {

  public:

    external_part ( drift::PartGraph& pg,
		    redox::Redox& rdx,
		    const bool now = false );
    external_part ( drift::PartGraph& pg,
		    redox::Redox& rdx,
		    const Json::Value& storage_meta,
		    bool now = false );
    external_part ( const boost::uuids::uuid&,
		    drift::PartGraph& pg,
		    redox::Redox& rdx,
		    bool now = false );
    virtual ~external_part() = default;

    virtual void load();
    virtual void store();

  public:

    Json::Value storage_meta_;

  private:

    external_part& operator= ( const external_part& );
    external_part ( const external_part& );

  };

  
  class immediate_part : public part {

  public:

    immediate_part ( drift::PartGraph& pg,
		     redox::Redox& rdx,
		     const bool now = false );
    immediate_part ( const boost::uuids::uuid& tag,
		     drift::PartGraph& pg,
		     redox::Redox& rdx,
		     const bool now = false );
    virtual ~immediate_part() = default;

    virtual void load();
    virtual void store();

    template<typename X>
    part&
    operator= (const X& x) { immediate_ = x ; }
    
  public:

    Json::Value immediate_;

  private:

    immediate_part& operator= ( const immediate_part& );
    immediate_part ( const immediate_part& );

  };


}


#endif // __PART_H__




    
