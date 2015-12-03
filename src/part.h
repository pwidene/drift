/* -*- mode: c++; -*- */
#ifndef __PART_H__
#define __PART_H__

#include <string>
#include <unordered_map>
#include <bitset>

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/chrono.hpp"
#include "boost/adjacency_list.hpp"
#include "boost/tuple/tuple.hpp"


#include "json.h"

#include "internal.h"

namespace drift {

  class part : public proactive {

  public:

    part ( const bool now = false );
    part ( const boost::uuids::uuid&, bool now = false );
    virtual ~part();

    virtual void load();
    virtual void store();
    virtual void adopt( part& );
    virtual void abandon ( part& );
    virtual void abandon();

  public:

    std::unordered_map < part*, std::string > parts_;
    std::string name_;

    // My UUID
    boost::uuids::uuid tag_;
    
    // Pointer to my vertex in the service's part graph
    boost::vertex_descriptor v_;

    // Creation and modification times
    boost::chrono::system_clock::time_point ctime_, mtime_;

  protected:

    static const std::string& get_n4j_rest_uri();
    virtual void json_props ( web::json::value& );

    std::bitset<8> dirty_;
    const short int Properties = 0;
    const short int External = 1;
    const short int Immediate = 2;

    const std::string n4j_rest_uri = "http://localhost:7474/db/data/";
    const std::string& get_n4j_rest_uri() const { return n4j_rest_uri; };

    // Reference to the whole part graph owned by the service instance
    // who created me
    drift::PartGraph& pgraph_;

  private:
    
    part& operator= ( const part& );
    part ( const part& );

  };

  
  class external_part : public part {

  public:

    external_part ( const bool now = false );
    external_part ( const web::json::value& storage_meta );
    external_part ( const boost::uuids::uuid&, bool now = false );
    virtual ~external_part();

    virtual void load();
    virtual void store();

  public:

    web::json::value storage_meta_;

  protected:

    virtual void json_props ( web::json::value& );

  private:

    external_part& operator= ( const external_part& );
    external_part ( const external_part& );

  };

  class immediate_part : public part {

  public:

    immediate_part ( const bool now = false );
    immediate_part ( const boost::uuids::uuid&, bool now = false );
    virtual ~immediate_part();

    virtual void load();
    virtual void store();

    part& operator= ( unsigned long u ) { immediate_["unsigned-long"] = web::json::value::parse ( std::to_string ( u ) ); }
    part& operator= ( long l ) { immediate_["long"] = web::json::value::parse ( std::to_string ( l ) ); }
    part& operator= ( std::string& s ) { immediate_["string"] = web::json::value::parse ( s ); };
    part& operator= ( double d ) { immediate_["double"] = web::json::value::parse ( std::to_string ( d ) ); };

  public:

    web::json::value immediate_;

  protected:

    virtual void json_props ( web::json::value& );

  private:

    immediate_part& operator= ( const immediate_part& );
    immediate_part ( const immediate_part& );

  };


}


#endif // __PART_H__




    
