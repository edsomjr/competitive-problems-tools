#include <iostream>
#include "ini.h"

using namespace std;

int main()
{
    Ini ini = Ini::load("teste.ini");

    cout << "Hello World\n";

    return 0;
}

