#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class CLI {
    public:
        vector<string> args;

        CLI(char *args[]);

        int checkArgs();
};

#endif