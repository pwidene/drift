
#include "evpath.h"

#include "formats.h"


  static FMField FMfields_terminator = {(const char*)0, (const char*)0, 0, 0};
  static FMStructDescRec FMStructDescRec_list_terminator = {(char*)0, (FMFieldList)0};

FMField drift::heartbeat_fields[] =
  {
    {"ts", "integer", sizeof(long), FMOffset(drift::heartbeat_ptr, ts)},
    {"flags", "integer", sizeof(long), FMOffset(drift::heartbeat_ptr, flags)},
    FMfields_terminator
  };
FMStructDescRec drift::heartbeat_formats[] =
  {
    {"heartbeat", drift::heartbeat_fields, sizeof(drift::heartbeat), (FMOptInfo*)0 },
    FMStructDescRec_list_terminator
  };

FMField drift::advert_fields[] = 
  {
    {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::advert_ptr, meta)},
    {"service_endpoint", "string", sizeof(char*), FMOffset(drift::advert_ptr, service_endpoint)},
    {"endpoint_stone", "integer", sizeof(int), FMOffset(drift::advert_ptr, endpoint_stone)},
    FMfields_terminator
  };

FMStructDescRec drift::advert_formats[] =
  {
    {"advert", drift::advert_fields, sizeof(drift::advert), (FMOptInfo*)0 },
    {"heartbeat", drift::heartbeat_fields, sizeof(drift::heartbeat), (FMOptInfo*)0 },
    FMStructDescRec_list_terminator
  };


FMField drift::put_i_immediate_fields[] =
    {
      {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::put_i_immediate_ptr, meta)},
      {"val", "integer", sizeof(unsigned long), FMOffset(drift::put_i_immediate_ptr, val)},
      {"path", "string", sizeof(char*), FMOffset(drift::put_i_immediate_ptr, path)},
      FMfields_terminator
    };
FMStructDescRec drift::put_i_immediate_formats[] =
    {
      {"put_i_immediate", drift::put_i_immediate_fields, sizeof(drift::put_i_immediate), (FMOptInfo*)0 },
      {"heartbeat", drift::heartbeat_fields, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };


FMField drift::put_d_immediate_fields[] =
    {
      {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::put_d_immediate_ptr, meta)},
      {"val", "float", sizeof(double), FMOffset(drift::put_d_immediate_ptr, val)},
      {"path", "string", sizeof(char*), FMOffset(drift::put_d_immediate_ptr, path)},
      FMfields_terminator
    };
FMStructDescRec drift::put_d_immediate_formats[] =
    {
      {"put_d_immediate", drift::put_d_immediate_fields, sizeof(drift::put_d_immediate), (FMOptInfo*)0 },
      {"heartbeat", drift::heartbeat_fields, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

FMField drift::put_s_immediate_fields[] =
    {
      {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::put_s_immediate_ptr, meta)},
      {"val", "string", sizeof(char*), FMOffset(drift::put_s_immediate_ptr, val)},
      {"path", "string", sizeof(char*), FMOffset(drift::put_s_immediate_ptr, path)},
      FMfields_terminator
    };
FMStructDescRec drift::put_s_immediate_formats[] =
    {
      {"put_s_immediate", drift::put_s_immediate_fields, sizeof(drift::put_s_immediate), (FMOptInfo*)0 },
      {"heartbeat", drift::heartbeat_fields, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

FMField drift::put_json_immediate_fields[] =
    {
      {"meta", "heartbeat", sizeof(drift::heartbeat), FMOffset(drift::put_json_immediate_ptr, meta)},
      {"val", "string", sizeof(char*), FMOffset(drift::put_json_immediate_ptr, val)},
      {"path", "string", sizeof(char*), FMOffset(drift::put_json_immediate_ptr, path)},
      FMfields_terminator
    };
FMStructDescRec drift::put_json_immediate_formats[] =
    {
      {"put_json_immediate", drift::put_json_immediate_fields, sizeof(drift::put_json_immediate), (FMOptInfo*)0 },
      {"heartbeat", drift::heartbeat_fields, sizeof(drift::heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

FMField drift::simple_part_fields[] = 
    {
      {"payload", "string", sizeof(char*), FMOffset(drift::simple_part_ptr, payload)},
      {"store_type", "integer", sizeof(unsigned long), FMOffset(drift::simple_part_ptr, store_type)},
      {"metadata", "string", sizeof(char*), FMOffset(drift::simple_part_ptr, metadata)},
      FMfields_terminator
    };
FMStructDescRec drift::simple_part_formats[] =
    {
      {"simple_part", drift::simple_part_fields, sizeof(drift::simple_part), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

FMField drift::complex_part_fields[] = 
    {
      {"count", "integer", sizeof(unsigned long), FMOffset(drift::complex_part_ptr, count)},
      {"parts", "simple_part[count]", sizeof(drift::simple_part), FMOffset(drift::complex_part_ptr, parts)},
      {"metadata", "string", sizeof(char*), FMOffset(drift::complex_part_ptr, metadata)},
      FMfields_terminator
    };
FMStructDescRec drift::complex_part_formats[] = 
    {
      {"complex_part", drift::complex_part_fields, sizeof(drift::complex_part), (FMOptInfo*)0 },
      {"simple_part", drift::simple_part_fields, sizeof(drift::simple_part), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

FMField drift::simple_part_xfer_fields[] =
    {
      {"flags", "integer", sizeof(unsigned long), FMOffset(drift::simple_part_xfer_ptr, flags)},
      {"part", "simple_part", sizeof(drift::simple_part), FMOffset(drift::simple_part_xfer_ptr, part)},
      {"index_name", "string", sizeof(char*), FMOffset(drift::simple_part_xfer_ptr, index_name)},
      {"index_spec", "string", sizeof(char*), FMOffset(drift::simple_part_xfer_ptr, index_spec)},
      FMfields_terminator
    };
FMStructDescRec drift::simple_part_xfer_formats[] =
    {
      {"simple_part_xfer", drift::simple_part_xfer_fields, sizeof(drift::simple_part_xfer), (FMOptInfo*)0 },
      {"simple_part", drift::simple_part_fields, sizeof(drift::simple_part), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

FMField drift::complex_part_xfer_fields[] = 
    {
      {"flags", "integer", sizeof(unsigned long), FMOffset(drift::complex_part_xfer_ptr, flags) },
      {"part", "complex_part", sizeof(drift::complex_part), FMOffset(drift::complex_part_xfer_ptr, part) },
      {"index_name", "string", sizeof(char*), FMOffset(drift::complex_part_xfer_ptr, index_name)},
      {"index_spec", "string", sizeof(char*), FMOffset(drift::complex_part_xfer_ptr, index_spec)},
      FMfields_terminator
    };
FMStructDescRec drift::complex_part_xfer_formats[] =
    {
      {"complex_part_xfer", drift::complex_part_xfer_fields, sizeof(drift::complex_part_xfer), (FMOptInfo*)0 },
      {"complex_part", drift::complex_part_fields, sizeof(drift::complex_part), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

