#ifndef INI_H
#define INI_H

#include <string>
#include <map>

using namespace std;

class Ini
{
    map<string, map<string, string> > _sections;

    Ini() {}

    void add(const string& section, const string& key, const string& value);

public:

    static Ini load(const string& filepath);

    string value(const string& section, const string& key, const string& default_value = "");
};

#endif
