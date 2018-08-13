#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; ++i)
        cout << "Arg = " << argv[i] << endl;

    ofstream out("spec.txt");

    out << "Hello World\n";

    out.close();

    return 0;
}

