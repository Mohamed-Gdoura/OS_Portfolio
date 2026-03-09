#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Function to read input from file
void readInput(
    string filename,
    int &n,
    int &m,
    vector<int> &E,
    vector<vector<int>> &C,
    vector<vector<int>> &R
) {

    cout << "Reading input file: " << filename << endl;

    ifstream file;
    file.open(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << endl;
        exit(1);
    }

    file >> n;
    file >> m;

    E.resize(m);

    for (int i = 0; i < m; i++)
        file >> E[i];

    C.resize(n, vector<int>(m));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            file >> C[i][j];

    R.resize(n, vector<int>(m));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            file >> R[i][j];

    file.close();
}

// Compute Available vector
vector<int> computeAvailable(
    int n,
    int m,
    vector<int> &E,
    vector<vector<int>> &C
) {

    vector<int> allocated(m, 0);

    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++)
            allocated[j] += C[i][j];

    vector<int> A(m);

    for (int j = 0; j < m; j++)
        A[j] = E[j] - allocated[j];

    return A;
}

// Check if process request <= available resources
bool canExecute(vector<int> &request, vector<int> &work) {

    for (int i = 0; i < request.size(); i++)
        if (request[i] > work[i])
            return false;

    return true;
}

// Deadlock detection algorithm
void detectDeadlock(
    int n,
    int m,
    vector<int> &A,
    vector<vector<int>> &C,
    vector<vector<int>> &R
) {

    vector<int> work = A;
    vector<bool> finish(n, false);

    bool progress = true;

    while (progress) {

        progress = false;

        for (int i = 0; i < n; i++) {

            if (!finish[i] && canExecute(R[i], work)) {

                for (int j = 0; j < m; j++)
                    work[j] += C[i][j];

                finish[i] = true;
                progress = true;
            }
        }
    }

    bool deadlock = false;

    for (int i = 0; i < n; i++)
        if (!finish[i])
            deadlock = true;

    if (!deadlock) {

        cout << "No deadlock detected.\n";
        cout << "All processes can complete.\n";

    } else {

        cout << "Deadlock detected!\n";
        cout << "Deadlocked processes: ";

        for (int i = 0; i < n; i++)
            if (!finish[i])
                cout << "P" << i << " ";

        cout << endl;
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Usage: ./deadlock <input_file>" << endl;
        return 1;
    }

    string filename = argv[1];

    int n, m;

    vector<int> E;
    vector<vector<int>> C;
    vector<vector<int>> R;

    readInput(filename, n, m, E, C, R);

    vector<int> A = computeAvailable(n, m, E, C);

    detectDeadlock(n, m, A, C, R);

    cout << "Available resources: ";
    for (int x : A) cout << x << " ";
    cout << endl;

    return 0;
}