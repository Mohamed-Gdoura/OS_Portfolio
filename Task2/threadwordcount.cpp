#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <pthread.h>
#include <cctype>

using namespace std;

// Structure to pass data to thread
struct ThreadData {
    int id;
    int start;
    int end;

    int wordCount;

    vector<string>* words;
    unordered_map<string, int>* localMap;
};

// Convert word to lowercase
string toLower(const string& s) {
    string result;

    for (char c : s) {
        result += tolower(c);
    }

    return result;
}

// Thread function
void* worker(void* arg) {

    ThreadData* data = (ThreadData*) arg;

    int count = 0;

    // Count words
    for (int i = data->start; i < data->end; i++) {

        string word = (*(data->words))[i];

        (*(data->localMap))[word]++;
        count++;
    }

    // Save count
    data->wordCount = count;

    pthread_exit(NULL);
}

int main() {

    // Number of threads
    const int N = 4;

    // Open file
    ifstream file("test.txt");

    if (!file.is_open()) {
        cout << "Error: Cannot open test.txt" << endl;
        return 1;
    }

    // Read words
    vector<string> words;
    string word;

    while (file >> word) {

        word = toLower(word);

        if (!word.empty()) {
            words.push_back(word);
        }
    }

    file.close();

    cout << "Total words: " << words.size() << endl;

    // Prepare threads
    pthread_t threads[N];
    ThreadData data[N];

    vector<unordered_map<string, int>> threadMaps(N);

    int total = words.size();
    int chunk = total / N;

    // Create threads
    for (int i = 0; i < N; i++) {

        data[i].id = i;
        data[i].start = i * chunk;

        if (i == N - 1)
            data[i].end = total;
        else
            data[i].end = (i + 1) * chunk;

        data[i].wordCount = 0;
        data[i].words = &words;
        data[i].localMap = &threadMaps[i];

        pthread_create(&threads[i], NULL, worker, &data[i]);
    }

    // Wait for threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print results in order
    for (int i = 0; i < N; i++) {

        cout << "\nThread " << data[i].id
             << " word count = " << data[i].wordCount << endl;

        cout << "Word Frequencies:\n";

        for (auto& pair : threadMaps[i]) {
            cout << pair.first << " : " << pair.second << endl;
        }
    }

    return 0;
}
