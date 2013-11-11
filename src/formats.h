#ifndef _FORMATS_H_
#define _FORMATS_H_

namespace drift {  

  typedef struct _heartbeat_s {
    unsigned long ts;
    unsigned long flags;
  } heartbeat, *heartbeat_ptr;

  extern FMField heartbeat_fields[];
  extern FMStructDescRec heartbeat_formats[];

  typedef struct _put_i_immediate {
    heartbeat meta;
    unsigned long val;
    char *path;
  } put_i_immediate, *put_i_immediate_ptr;

  extern FMField put_i_immediate_fields[];
  extern FMStructDescRec put_i_immediate_formats[];

  typedef struct _put_d_immediate {
    heartbeat meta;
    double val;
    char *path;
  } put_d_immediate, *put_d_immediate_ptr;

  extern FMField put_d_immediate_fields[];
  extern FMStructDescRec put_d_immediate_formats[];

  typedef struct _put_s_immediate {
    heartbeat meta;
    char *val;
    char *path;
  } put_s_immediate, *put_s_immediate_ptr;

  extern FMField put_s_immediate_fields[];
  extern FMStructDescRec put_s_immediate_formats[];

  typedef struct _put_json_immediate {
    heartbeat meta;
    char *val;
    char *path;
  } put_json_immediate, *put_json_immediate_ptr;

  extern FMField put_json_immediate_fields[];
  extern FMStructDescRec put_json_immediate_formats[];

  typedef struct _simple_part {
    char *payload;
    unsigned long store_type;
    char *metadata;
  } simple_part, *simple_part_ptr;

  extern FMField simple_part_fields[];
  extern FMStructDescRec simple_part_formats[];

  typedef struct _complex_part {
    unsigned long count;
    struct _simple_part *parts;
    char *metadata;
  } complex_part, *complex_part_ptr;
  
  extern FMField complex_part_fields[];
  extern FMStructDescRec complex_part_formats[];

  typedef struct _simple_part_xfer {
    unsigned long flags;
    struct _simple_part part;
    char* index_name;
    char* index_spec;
  } simple_part_xfer, *simple_part_xfer_ptr;

  extern FMField simple_part_xfer_fields[];
  extern FMStructDescRec simple_part_xfer_formats[];

  typedef struct _complex_part_xfer {
    unsigned long flags;
    struct _complex_part part;
    char *index_name;
    char *index_spec;
  } complex_part_xfer, *complex_part_xfer_ptr;

  extern FMField complex_part_xfer_fields[];
  extern FMStructDescRec complex_part_xfer_formats[];


}


#endif /* _FORMATS_H_ */
