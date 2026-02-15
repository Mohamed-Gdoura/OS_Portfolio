#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>

using namespace std;

// Split input into words
vector<string> tokenize(string input) {
    vector<string> tokens;
    string word;
    stringstream ss(input);

    while (ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}

// Convert vector<string> to char*[]
vector<char*> toCharArray(vector<string>& args) {
    vector<char*> result;

    for (string& s : args)
        result.push_back(&s[0]);

    result.push_back(NULL);

    return result;
}

int main(int argc, char* argv[]) {

    // Batch mode
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }

    string input;
    char cwd[1024];

    while (true) {

        getcwd(cwd, sizeof(cwd));
        cout << cwd << "> ";

        if (!getline(cin, input))
            break;

        vector<string> args = tokenize(input);

        if (args.size() == 0)
            continue;

        // quit
        if (args[0] == "quit")
            break;

        // cd
        else if (args[0] == "cd") {
            if (args.size() == 1) {
                cout << cwd << endl;
            }
            else {
                if (chdir(args[1].c_str()) != 0)
                    perror("cd error");

                setenv("PWD", getcwd(cwd, sizeof(cwd)), 1);
            }
        }

        // echo
        else if (args[0] == "echo") {
            for (size_t i = 1; i < args.size(); i++)
                cout << args[i] << " ";

            cout << endl;
        }

        // environ
        else if (args[0] == "environ") {
            extern char** environ;
            char** env = environ;

            while (*env) {
                cout << *env << endl;
                env++;
            }
        }

        // dir
        else if (args[0] == "dir") {
            string cmd = "ls ";

            if (args.size() > 1)
                cmd += args[1];
            else
                cmd += ".";

            system(cmd.c_str());
        }

        // set
        else if (args[0] == "set") {
            if (args.size() >= 3) {
                setenv(args[1].c_str(), args[2].c_str(), 1);
            }
            else {
                cout << "Usage: set VAR VALUE\n";
            }
        }

        // pause
        else if (args[0] == "pause") {
            cout << "Press Enter to continue...";
            cin.get();
        }

        // help
        else if (args[0] == "help") {
            system("more help.txt");
        }

        // External commands
        else {

            bool background = false;
            string inputFile, outputFile;
            bool append = false;

            // Check for &
            if (args.back() == "&") {
                background = true;
                args.pop_back();
            }

            // Handle redirection
            for (size_t i = 0; i < args.size(); i++) {

                if (args[i] == "<") {
                    inputFile = args[i + 1];
                    args.erase(args.begin() + i, args.begin() + i + 2);
                    i--;
                }

                else if (args[i] == ">") {
                    outputFile = args[i + 1];
                    append = false;
                    args.erase(args.begin() + i, args.begin() + i + 2);
                    i--;
                }

                else if (args[i] == ">>") {
                    outputFile = args[i + 1];
                    append = true;
                    args.erase(args.begin() + i, args.begin() + i + 2);
                    i--;
                }
            }

            pid_t pid = fork();

            if (pid == 0) {

                // Input redirection
                if (!inputFile.empty()) {
                    int fd = open(inputFile.c_str(), O_RDONLY);
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                // Output redirection
                if (!outputFile.empty()) {

                    int fd;

                    if (append)
                        fd = open(outputFile.c_str(),
                                  O_WRONLY | O_CREAT | O_APPEND, 0644);
                    else
                        fd = open(outputFile.c_str(),
                                  O_WRONLY | O_CREAT | O_TRUNC, 0644);

                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                vector<char*> cargs = toCharArray(args);

                execvp(cargs[0], cargs.data());

                perror("Command error");
                exit(1);
            }

            else {

                if (!background)
                    wait(NULL);
            }
        }
    }

    return 0;
}