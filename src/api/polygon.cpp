#include "api/polygon.h"

namespace cptools::api::polygon {

    bool test_connection(const Credentials& creds) {
        // TODO: http get
        return creds.key != "" && creds.secret != "";
    }

}