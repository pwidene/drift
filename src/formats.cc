
#include "evpath.h"

#include "formats.h"

static FMField FMField_list_terminator = {(const char*)0, (const char*)0, 0, 0};
static FMStructDescRec FMStructDescRec_list_terminator = {(char*)0, (FMFieldList)0};

namespace drift {

  FMField heartbeat_field_list[] =
    {
      {"ts", "integer", sizeof(long), FMOffset(heartbeat_ptr, ts)},
      {"flags", "integer", sizeof(long), FMOffset(heartbeat_ptr, flags)},
      FMField_list_terminator
    };
  FMStructDescRec heartbeat_format_list[] =
    {
      {"heartbeat", heartbeat_field_list, sizeof(heartbeat), (FMOptInfo*)0 },
      FMStructDescRec_list_terminator
    };

}


