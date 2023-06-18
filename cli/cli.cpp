#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "cli.h"
#include "../io.h"

using namespace std;

int main(int argc, char* argv[]) {
    CLI cli(argv);
    if(cli.checkArgs() == 1) {
        return 1;
    }

    IO io(cli.args[1]);
    io.reportFileSize();

}

CLI::CLI(char *args[]) {
    int i = 0;
    while (args[i] != nullptr) {
        this->args.push_back(args[i]);
        i++;
    }
}

int CLI::checkArgs() {
    if(this->args.size() < 2) {
        cout << "Usage: " << this->args[0] << " <filename>" << endl;
        return 1;
    }

    if(this->args[1].substr(this->args[1].length() - 4, this->args[1].length()) != ".txt") {
        cout << "Error: File must be a text file (.txt)" << endl;
        return 1;
    }

    return 0;
}