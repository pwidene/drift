
#include <string>
#include <boost/version.hpp>

#include "mongo/util/version.h"

namespace mongo {
    const char * gitVersion() { return "b283169ae51328f3fb5e3c8a3c9017a2e42c148f"; }
    const char * compiledJSEngine() { return "V8"; }
    const char * allocator() { return "tcmalloc"; }
    const char * loaderFlags() { return "-fPIC -pthread -Wl,-z,now -rdynamic"; }
    const char * compilerFlags() { return "-Wnon-virtual-dtor -Woverloaded-virtual -fPIC -fno-strict-aliasing -ggdb -pthread -Wall -Wsign-compare -Wno-unknown-pragmas -Winvalid-pch -Werror -pipe -O3 -Wno-unused-function -Wno-deprecated-declarations -fno-builtin-memcmp"; }
    std::string sysInfo() { return "Linux ip-10-2-29-40 2.6.21.7-2.ec2.v1.2.fc8xen #1 SMP Fri Nov 20 17:48:28 EST 2009 x86_64 BOOST_LIB_VERSION=" BOOST_LIB_VERSION ; }
}  // namespace mongo
