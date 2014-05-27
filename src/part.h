#ifndef __PART_H__
#define __PART_H__

#include <string>
#include <unordered_map>
#include <bitset>

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/chrono.hpp"

#include "json.h"

#include "internal.h"

namespace drift {

  class part {

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
    boost::uuids::uuid tag_;
    std::string node_uri_;
    boost::chrono::system_clock::time_point ctime_, mtime_;

  protected:

    static const std::string& get_n4j_rest_uri();
    virtual void json_props ( web::json::value& );

    std::bitset<8> dirty_;
    const short int Properties = 0;
    const short int External = 1;
    const short int Immediate = 2;

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

    part& operator= ( unsigned long i ) { immediate_.i_ = i; };
    part& operator= ( std::string& s ) { immediate_.s_ = s; };
    part& operator= ( double d ) { immediate_.d_ = d; };

  public:

    union {
      unsigned long u_;
      long l_;
      std::string s_;
      double d_;
    } immediate_;

  protected:

    virtual void json_props ( web::json::value& );

  private:

    immediate_part& operator= ( const immediate_part& );
    immediate_part ( const immediate_part& );

  };


}

/*
 *  
#endif // __PART_H__

    
