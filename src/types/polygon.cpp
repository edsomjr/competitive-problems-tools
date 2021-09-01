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

void from_json(const nlohmann::json &j, Statement &s) {
    auto language = j["language"].get<std::string>();
    language = util::lower_string(language);

    if (language == "portuguese")
        s.language = "pt_BR";
    else if (language == "english")
        s.language = "en_US";
    else
        s.language = language;

    s.title = j["name"].get<std::string>();
}

void from_json(const nlohmann::json &j, Information &i) {
    i.memory_limit = j["memoryLimit"].get<int>();
    i.time_limit = j["timeLimit"].get<int>();
}

void from_json(const nlohmann::json &j, Test &t) {
    t.index = j["index"].get<int>();
    t.manual = j["manual"].get<bool>();

    if (t.manual) {
        t.use_in_statements = j["useInStatements"].get<bool>();
        t.input = j["input"].get<std::string>();
        t.script_line = "";
    } else {
        t.script_line = j["scriptLine"].get<std::string>();
        t.use_in_statements = false;
        t.input = "";
    }
}

} // namespace cptools::types::polygon