#ifndef __INTERNAL_H__
#define __INTERNAL_H__ 

#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/sinks/sync_frontend.hpp"
#include "boost/log/sinks/text_ostream_backend.hpp"
#include "boost/log/sources/severity_feature.hpp"
#include "boost/log/sources/severity_logger.hpp"
#include "boost/log/utility/setup/console.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"

namespace logging = boost::log;

/*  Logging severity levels */

namespace drift {

  enum severity_level { debug, warning, error, info, critical };

}

#endif // __INTERNAL_H__
