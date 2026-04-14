#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <ctime>

extern "C" {
#include "des.h"
}

using namespace std;

#define NUM_PASSWORDS 10

void prepareBlock(const string &password, unsigned char block[8]) {
    memset(block, 0, 8);

    for (int i = 0; i < 8 && i < (int)password.length(); i++) {
        block[i] = password[i];
    }
}

unsigned short generateSalt() {
    return rand() % 65536;
}

void buildKey(unsigned short salt, unsigned char key[8]) {
    key[0] = salt & 0xFF;
    key[1] = (salt >> 8) & 0xFF;

    for (int i = 2; i < 8; i++) {
        key[i] = (key[0] + key[1] + i) & 0xFF;
    }
}

void encrypt25(unsigned char block[8], unsigned char key[8]) {
    unsigned char temp[8];

    for (int i = 0; i < 25; i++) {
        key_set key_sets[17];

        generate_sub_keys(key, key_sets);
        process_message(block, temp, key_sets, ENCRYPTION_MODE);

        memcpy(block, temp, 8);
    }
}

void printHex(unsigned char block[8]) {
    for (int i = 0; i < 8; i++) {
        printf("%02X", block[i]);
    }
}

int main() {
    srand(time(0));

    string passwords[NUM_PASSWORDS] = {
        "alpha123", "beta456", "gamma789", "delta321", "omega999",
        "pass111", "hello222", "secure33", "unix4444", "crypt555"
    };

    unsigned short salts[NUM_PASSWORDS];
    unsigned char hashes[NUM_PASSWORDS][8];

    cout << "Generated Encrypted Passwords\n";

    for (int i = 0; i < NUM_PASSWORDS; i++) {
        salts[i] = generateSalt();

        unsigned char block[8];
        unsigned char key[8];

        prepareBlock(passwords[i], block);
        buildKey(salts[i], key);

        encrypt25(block, key);

        memcpy(hashes[i], block, 8);

        cout << setw(2) << setfill('0') << i + 1 << ". "
             << hex << setw(4) << setfill('0') << salts[i]
             << ":";

        printHex(hashes[i]);
        cout << endl;
    }

    string input;
    cout << "\nEnter password to validate: ";
    cin >> input;

    bool found = false;

    for (int i = 0; i < NUM_PASSWORDS; i++) {
        unsigned char block[8];
        unsigned char key[8];

        prepareBlock(input, block);
        buildKey(salts[i], key);

        encrypt25(block, key);

        if (memcmp(block, hashes[i], 8) == 0) {
            cout << "Password VALID (match " << i + 1 << ")\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Password INVALID\n";
    }

    return 0;
}