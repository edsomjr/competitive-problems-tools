#include <iostream>
#include <fstream>

using namespace std;

#include "ini.h"
#include "util.h"

void
Ini::add(const string& section, const string& key, const string& value)
{
    _sections[section][key] = value;
}

string
Ini::value(const string& section, const string& key, const string& default_value)
{
    auto it = _sections.find(section);

    if (it == _sections.end())
        return default_value;

    auto jt = it->second.find(key);

    return jt == it->second.end() ? default_value : jt->second;
}

Ini
Ini::load(const string& filepath)
{
    ifstream in(filepath);
    Ini ini;

    if (not in)
    {
        cerr << "Couldn't find file '" << filepath << "'\n";
        return ini;
    }

    string line, section = "";

    while (getline(in, line))
    {
        line = strip(line);

        if (line.size() == 0)
            continue;

        auto start = line.find('[');

        if (start != string::npos)
        {
            auto end = line.find(']');

            if (start + 1 < end - 1)
            {
                section = line.substr(start + 1, end - start - 1);
                continue;
            } else
            {
                cerr << "Invalid line: [" << line << "]\n";
                return ini;
            }
        } 

        auto mid = line.find('=');

        if (mid == string::npos)
        {
            cerr << "Invalid pair key/value: [" << line << "]\n";
            return ini;
        }

        auto key = strip(line.substr(0, mid));
        auto value = strip(line.substr(mid + 1));

        ini.add(section, key, value);
    }

    in.close();

    return ini;
}
