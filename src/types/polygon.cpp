#include <string>

#include "types/polygon.h"
#include "util.h"

namespace cptools::types::polygon {

void from_json(const nlohmann::json &j, Solution &s) {
    auto tag_value = j["tag"].get<std::string>();
    tag_value = util::lower_string(tag_value);

    if (tag_value == "ma")
        s.tag = "default";
    else if (tag_value == "tl")
        s.tag = "tle";
    else if (tag_value == "ok")
        s.tag = "ac";
    else
        s.tag = tag_value;

    j.at("name").get_to(s.name);
    j.at("modificationTimeSeconds").get_to(s.modification_time);
}

} // namespace cptools::types::polygon