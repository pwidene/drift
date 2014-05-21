#ifndef __PART_H__
#define __PART_H__

#include <string>
#include <set>

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/chrono.hpp"

#include "json.h"

#include "internal.h"

namespace drift {

  class part {

  public:

    part(const bool now = false);
    part(const boost::uuids::uuid&, bool now = false);
    virtual ~part();

    void load();
    void store();

    std::string name_;
    boost::uuids::uuid tag_;
    std::string node_uri_;
    boost::chrono::system_clock::time_point ctime_, mtime_;

    union {
      unsigned long i_;
      std::string s_;
      double d_;
      std::string json_;
    } immediate_;

    std::set < std::pair<std::string, part*> > parts_;

    static const std::string& get_n4j_rest_uri();

    void json_props ( web::json::value& );

    adopt( part& );
    abandon ( part& );
    abandon();

  private:
    
    part& operator= ( const part& );
    part ( const part& );

  };

}


#endif // __PART_H__

    
