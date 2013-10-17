#ifndef _FORMATS_H_
#define _FORMATS_H_

namespace drift {  

  typedef struct _heartbeat_s {
    unsigned long ts;
    unsigned long flags;
  } heartbeat, *heartbeat_ptr;

  extern FMField heartbeat_field_list[];
  extern FMStructDescRec heartbeat_format_list[];

  typedef struct _put_immediate_s {
    unsigned long ts;
    unsigned long flags;
    unsigned long val;
  } put_immediate, *put_immediate_ptr;

}


#endif /* _FORMATS_H_ */
