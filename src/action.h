#ifndef __ACTION_H__
#define __ACTION_H__

#include "atl.h"
#include "evpath.h"


#include "internal.h"

namespace drift {

  class action {
  public:
    static int put_immediate_action ( CManager, void*, void*, attr_list );
    static int get_immediate_action ( CManager, void*, void*, attr_list );
    static int simple_part_xfer_action ( CManager, void*, void*, attr_list );
    static int complex_part_xfer_action ( CManager, void*, void*, attr_list );
    
    static void action_setup();

    typedef enum { immed_int, immed_double, immed_string, immed_json } immed_type_t;

    class immed_type {
    public:
      immed_type ( const immed_type_t& it ) : type_ ( it ) {}
      immed_type_t type_;
    };


  protected:
    static EVstone action_stone_;

  private:
    action(const action&);
    action& operator=(const action&);
  };

}

#endif // __ACTION_H__
