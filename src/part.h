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
    part ( web::json::value& storage_meta );
    part ( const boost::uuids::uuid&, bool now = false );
    virtual ~part();

    void load();
    void store();
    void adopt( part& );
    void abandon ( part& );
    void abandon();

    part& operator= ( unsigned long i ) { immediate_.i_ = i; };
    part& operator= ( std::string& s ) { immediate_.s_ = s; };
    part& operator= ( double d ) { immediate_.d_ = d; };

  public:

    std::unordered_map < part*, std::string > parts_;
    std::string name_;
    boost::uuids::uuid tag_;
    std::string node_uri_;
    boost::chrono::system_clock::time_point ctime_, mtime_;

    union {
      unsigned long i_;
      std::string s_;
      double d_;
    } immediate_;

    web::json::value storage_meta_;

  protected:

    static const std::string& get_n4j_rest_uri();
    void json_props ( web::json::value& );

    std::bitset<8> dirty_;
    const short int Properties = 0;
    const short int External = 1;
    const short int Immediate = 2;

  private:
    
    part& operator= ( const part& );
    part ( const part& );

  };

}

/*
 *  
#endif // __PART_H__

    
