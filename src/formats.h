#ifndef _FORMATS_H_
#define _FORMATS_H_

namespace drift {  

  typedef struct _heartbeat_s {
    unsigned long ts;
    unsigned long flags;
  } heartbeat, *heartbeat_ptr;

  extern FMField heartbeat_field_list[];
  extern FMStructDescRec heartbeat_format_list[];

  typedef struct _put_immediate_i {
    heartbeat meta;
    unsigned long val;
    char *path;
  } put_immediate_i, *put_immediate_i_ptr;

  extern FMField put_immediate_i_field_list[];
  extern FMStructDescRec put_immediate_i_format_list[];

  typedef struct _put_immediate_d {
    heartbeat meta;
    double val;
    char *path;
  } put_immediate_d, *put_immediate_d_ptr;

  extern FMField put_immediate_d_field_list[];
  extern FMStructDescRec put_immediate_d_format_list[];

  typedef struct _put_immediate_s {
    heartbeat meta;
    char *val;
    char *path;
  } put_immediate_s, *put_immediate_s_ptr;

  extern FMField put_immediate_s_field_list[];
  extern FMStructDescRec put_immediate_s_format_list[];

  typedef struct _put_immediate_json {
    heartbeat meta;
    char *val;
    char *path;
  } put_immediate_json, *put_immediate_json_ptr;

  extern FMField put_immediate_json_field_list[];
  extern FMStructDescRec put_immediate_json_format_list[];

  typedef struct _simple_part {
    char *payload;
    unsigned long store_type;
    char *metadata;
  } simple_part, *simple_part_ptr;

  extern FMField simple_part_field_list[];
  extern FMStructDescRec simple_part_format_list[];

  typedef struct _complex_part {
    unsigned long count;
    struct _simple_part *parts;
    char *metadata;
  } complex_part, *complex_part_ptr;
  
  extern FMField complex_part_field_list[];
  extern FMStructDescRec complex_part_format_list[];

  typedef struct _simple_part_xfer {
    unsigned long flags;
    struct _simple_part part;
    char* index_name;
    char* index_spec;
  } simple_part_xfer, *simple_part_xfer_ptr;

  extern FMField simple_part_xfer_field_list[];
  extern FMStructDescRec simple_part_xfer_format_list[];

  typedef struct _complex_part_xfer {
    unsigned long flags;
    struct _complex_part part;
    char *index_name;
    char *index_spec;
  } complex_part_xfer, *complex_part_xfer_ptr;

  extern FMField complex_part_xfer_field_list[];
  extern FMStructDescRec complex_part_xfer_format_list[];


}


#endif /* _FORMATS_H_ */
