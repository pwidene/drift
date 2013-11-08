
#include "evpath.h"

#include "formats.h"

static FMField FMField_list_terminator = {(const char*)0, (const char*)0, 0, 0};
static FMStructDescRec FMStructDescRec_list_terminator = {(char*)0, (FMFieldList)0};

  FMField heartbeat_field_list[] =
    {
      {"ts", "integer", sizeof(long), FMOffset(drift::heartbeat_ptr, ts)},
      {"flags", "integer", sizeof(long), FMOffset(drift::heartbeat_ptr, flags)},
      FMField_list_terminator
    };
  FMStructDescRec heartbeat_format_list[] =
    {
      {"heartbeat", drift::heartbeat_field_list, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

  FMField put_immediate_i_field_list[] =
    {
      {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::put_immediate_i_ptr, meta)},
      {"val", "integer", sizeof(unsigned long), FMOffset(drift::put_immediate_i_ptr, val)},
      {"path", "string", sizeof(char*), FMOffset(drift::put_immediate_i_ptr, path)},
      FMField_list_terminator
    };
  FMStructDescRec put_immedate_i_format_list[] =
    {
      {"put_immediate_i", drift::put_immediate_i_field_list, sizeof(drift::put_immediate_i), (FMOptInfo*)0 },
      {"heartbeat", drift::heartbeat_field_list, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

  FMField put_immediate_d_field_list[] =
    {
      {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::put_immediate_d_ptr, meta)},
      {"val", "float", sizeof(double), FMOffset(drift::put_immediate_d_ptr, val)},
      {"path", "string", sizeof(char*), FMOffset(drift::put_immediate_d_ptr, path)},
      FMField_list_terminator
    };
  FMStructDescRec put_immedate_d_format_list[] =
    {
      {"put_immediate_d", drift::put_immediate_d_field_list, sizeof(drift::put_immediate_d), (FMOptInfo*)0 },
      {"heartbeat", drift::heartbeat_field_list, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

  FMField put_immediate_s_field_list[] =
    {
      {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::put_immediate_s_ptr, meta)},
      {"val", "string", sizeof(char*), FMOffset(drift::put_immediate_s_ptr, val)},
      {"path", "string", sizeof(char*), FMOffset(drift::put_immediate_s_ptr, path)},
      FMField_list_terminator
    };
  FMStructDescRec put_immedate_s_format_list[] =
    {
      {"put_immediate_s", drift::put_immediate_s_field_list, sizeof(drift::put_immediate_s), (FMOptInfo*)0 },
      {"heartbeat", drift::heartbeat_field_list, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

  FMField put_immediate_json_field_list[] =
    {
      {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::put_immediate_json_ptr, meta)},
      {"val", "string", sizeof(char*), FMOffset(drift::put_immediate_json_ptr, val)},
      {"path", "string", sizeof(char*), FMOffset(drift::put_immediate_json_ptr, path)},
      FMField_list_terminator
    };
  FMStructDescRec put_immedate_json_format_list[] =
    {
      {"put_immediate_json", drift::put_immediate_json_field_list, sizeof(drift::put_immediate_json), (FMOptInfo*)0 },
      {"heartbeat", drift::heartbeat_field_list, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

  FMField simple_part_field_list[] = 
    {
      {"payload", "string", sizeof(char*), FMOffset(drift::simple_part_ptr, payload)},
      {"store_type", "integer", sizeof(unsigned long), FMOffset(drift::simple_part_ptr, store_type)},
      {"metadata", "string", sizeof(char*), FMOffset(drift::simple_part_ptr, metadata)},
      FMField_list_terminator
    };
  FMStructDescRec simple_part_format_list[] =
    {
      {"simple_part", drift::simple_part_field_list, sizeof(drift::simple_part), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

  FMField complex_part_field_list[] = 
    {
      {"count", "integer", sizeof(unsigned long), FMOffset(drift::complex_part_ptr, count)},
      {"parts", "simple_part[count]", sizeof(drift::simple_part), FMOffset(drift::complex_part_ptr, parts)},
      {"metadata", "string", sizeof(char*), FMOffset(drift::complex_part_ptr, metadata)},
      FMField_list_terminator
    };
  FMStructDescRec complex_part_format_list[] = 
    {
      {"complex_part", drift::complex_part_field_list, sizeof(drift::complex_part), (FMOptInfo*)0 },
      {"simple_part", drift::simple_part_field_list, sizeof(drift::simple_part), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

  FMField simple_part_xfer_field_list[] =
    {
      {"flags", "integer", sizeof(unsigned long), FMOffset(drift::simple_part_xfer_ptr, flags)},
      {"part", "simple_part", sizeof(drift::simple_part), FMOffset(drift::simple_part_xfer_ptr, part)},
      {"index_name", "string", sizeof(char*), FMOffset(drift::simple_part_xfer_ptr, index_name)},
      {"index_spec", "string", sizeof(char*), FMOffset(drift::simple_part_xfer_ptr, index_spec)},
      FMField_list_terminator
    };
  FMStructDescRec simple_part_xfer_format_list[] =
    {
      {"simple_part_xfer", drift::simple_part_xfer_field_list, sizeof(drift::simple_part_xfer), (FMOptInfo*)0 },
      {"simple_part", drift::simple_part_field_list, sizeof(drift::simple_part), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

  FMField complex_part_xfer_field_list[] = 
    {
      {"flags", "integer", sizeof(unsigned long), FMOffset(drift::complex_part_xfer_ptr, flags) },
      {"part", "complex_part", sizeof(drift::complex_part), FMOffset(drift::complex_part_xfer_ptr, part) },
      {"index_name", "string", sizeof(char*), FMOffset(drift::complex_part_xfer_ptr, index_name)},
      {"index_spec", "string", sizeof(char*), FMOffset(drift::complex_part_xfer_ptr, index_spec)},
      FMField_list_terminator
    };
  FMStructDescRec complex_part_xfer_format_list[] =
    {
      {"complex_part_xfer", drift::complex_part_xfer_field_list, sizeof(drift::complex_part_xfer), (FMOptInfo*)0 },
      {"complex_part", drift::complex_part_field_list, sizeof(drift::complex_part), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };
      
