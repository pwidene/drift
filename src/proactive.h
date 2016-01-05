/* -*- mode: c++; -*- */

#ifndef __PROACTIVE_H__
#define __PROACTIVE_H__


#include <string>

#include "json/json.h"

extern "C" {
#include "evpath.h"
};

#include "internal.h"

namespace drift {

  class proactive {

  public:

    virtual void set_up_stone();
    static void use_this_CM ( CManager cm );

    virtual EVstone add_target ( attr_list attrs, EVstone remote_stone );
    virtual void publish ( void *ev );

  public:

    EVstone stone_;
    EVaction split_;
    EVsource source_handle_;

  protected:

    proactive ( FMStructDescRec format_list[] );
    virtual ~proactive();

    FMStructDescRec *format_list_;
    static CManager server_cm_;

  };

};

#endif // inclusion guard

    
