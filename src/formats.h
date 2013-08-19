#ifndef _FORMATS_H_
#define _FORMATS_H_

namespace drift {

  typedef struct _heartbeat_ptr {
    unsigned long ts;
    unsigned long flags;
  } heartbeat, *heartbeat_ptr;

extern FMField heartbeat_field_list[];

}


#endif /* _FORMATS_H_ */
