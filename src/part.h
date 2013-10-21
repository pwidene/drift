#ifndef __PART_H__
#define __PART_H__

#include <string>
#include <vector>

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"

#include "internal.h"

namespace drift {

  class part {

  public:

    part(const bool now = false);
    part(const boost::uuids::uuid&, bool now = false);
    virtual ~part();

    void load();
    void create();
    void store();

    std::string name_;
    boost::uuids::uuid tag_;
    std::string node_uri_;

    union {
      unsigned long i_;
      std::string s_;
      double d_;
      std::string json_;
    } immediate_;

    std::vector<part*> partlist_;

    static const std::string& get_n4j_rest_uri() const;

  private:
    
    part& operator= ( const part& );
    part ( const part& );

  };

}


#endif // __PART_H__

    
