#include <iostream>
#include <filesystem>
#include <vector>
#include <map>

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[]) {

    // Check arguments
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <directory> <bin_width>\n";
        return 1;
    }

    fs::path startDir = argv[1];
    size_t binWidth = stoull(argv[2]);

    map<size_t, size_t> histogram; // bin -> count

    try {
        // Traverse directory recursively
        for (const auto& entry : fs::recursive_directory_iterator(startDir)) {
            if (entry.is_regular_file()) {

                size_t fileSize = entry.file_size();

                // Determine bin
                size_t bin = fileSize / binWidth;

                histogram[bin]++;
            }
        }

        // Print histogram
        cout << "\nFile Size Histogram (bin width = " << binWidth << " bytes)\n";
        cout << "-------------------------------------------------\n";

        for (const auto& [bin, count] : histogram) {
            size_t low = bin * binWidth;
            size_t high = (bin + 1) * binWidth - 1;

            cout << low << " - " << high << " : " << count << endl;
        }

    } catch (const fs::filesystem_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}