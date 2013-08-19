
#include "evpath.h"

#include "formats.h"

static FMField FMField_list_terminator = {(const char*)0, (const char*)0, 0, 0};

namespace drift {

  FMField heartbeat_field_list[] =
    {
      {"ts", "integer", sizeof(long), FMOffset(heartbeat_ptr, ts)},
      {"flags", "integer", sizeof(long), FMOffset(heartbeat_ptr, flags)},
      FMField_list_terminator
    };

}


