#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

using namespace std;

// Structure for each frame
struct Frame {
    int page;           // Page number
    unsigned char age;  // 8-bit aging counter
};

// Run aging algorithm for given number of frames
int simulateAging(vector<int>& refs, int numFrames) {

    vector<Frame> frames(numFrames);
    int pageFaults = 0;

    // Initialize frames
    for (int i = 0; i < numFrames; i++) {
        frames[i].page = -1; // Empty
        frames[i].age = 0;
    }

    // Process each reference
    for (int ref : refs) {

        bool hit = false;

        // Step 1: Shift all counters 
        for (int i = 0; i < numFrames; i++) {
            frames[i].age >>= 1;
        }

        // Step 2: Check if page is in memory
        for (int i = 0; i < numFrames; i++) {
            if (frames[i].page == ref) {

                // Page hit
                frames[i].age |= 128; // Set MSB
                hit = true;
                break;
            }
        }

        // Step 3: Page fault
        if (!hit) {

            pageFaults++;

            int victim = 0;
            unsigned char minAge = UCHAR_MAX;

            // Find smallest age
            for (int i = 0; i < numFrames; i++) {

                // Prefer empty frame
                if (frames[i].page == -1) {
                    victim = i;
                    break;
                }

                if (frames[i].age < minAge) {
                    minAge = frames[i].age;
                    victim = i;
                }
            }

            // Replace victim
            frames[victim].page = ref;
            frames[victim].age = 128; // Set MSB
        }
    }

    return pageFaults;
}

int main() {

    ifstream file("references.txt");

    if (!file) {
        cout << "Error: Cannot open references.txt\n";
        return 1;
    }

    vector<int> references;
    int x;

    // Read file 
    while (file >> x) {
        references.push_back(x);
    }

    file.close();

    if (references.size() == 0) {
        cout << "No references found!\n";
        return 1;
    }

    cout << "Total references: " << references.size() << endl << endl;

    // Frame sizes 
    int tests[] = {4, 6, 8, 10};

    for (int i = 0; i < 4; i++) {

        int frames = tests[i];

        int faults = simulateAging(references, frames);

        cout << "Test no." << (i + 1)
             << " on " << frames
             << " frames: "
             << faults
             << " faults" << endl;
    }

    return 0;
}