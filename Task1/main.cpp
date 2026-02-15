#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

int main() {
    string input;
    char cwd[1024];

    while (true) {
        getcwd(cwd, sizeof(cwd));

        cout << cwd << "> ";

        getline(cin, input);

        if (input == "quit") {
            break;
        }

        cout << "You typed: " << input << endl;
    }

    return 0;
}
