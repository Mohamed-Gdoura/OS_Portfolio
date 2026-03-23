#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int arrival;
    int burst;
    int remaining;
};

// Function to read processes from file
vector<Process> readProcesses(string filename) {
    ifstream file(filename);
    vector<Process> processes;

    if (!file) {
        cout << "Error opening file!" << endl;
        exit(1);
    }

    int n;
    file >> n;

    for (int i = 0; i < n; i++) {
        Process p;
        file >> p.id >> p.arrival >> p.burst;
        p.remaining = p.burst;
        processes.push_back(p);
    }

    file.close();
    return processes;
}

// FCFS Scheduling
double fcfs(vector<Process> processes) {
    sort(processes.begin(), processes.end(),
         [](Process a, Process b) { return a.arrival < b.arrival; });

    int time = 0;
    int total_waiting = 0;

    for (auto &p : processes) {
        if (time < p.arrival)
            time = p.arrival;

        int waiting = time - p.arrival;
        total_waiting += waiting;

        time += p.burst;
    }

    return (double)total_waiting / processes.size();
}

// SJF (Non-preemptive)
double sjf(vector<Process> processes) {
    int n = processes.size();
    vector<bool> done(n, false);

    int completed = 0, time = 0;
    int total_waiting = 0;

    while (completed < n) {
        int idx = -1;
        int min_burst = 1e9;

        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival <= time) {
                if (processes[i].burst < min_burst) {
                    min_burst = processes[i].burst;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        int waiting = time - processes[idx].arrival;
        total_waiting += waiting;

        time += processes[idx].burst;
        done[idx] = true;
        completed++;
    }

    return (double)total_waiting / n;
}

// Round Robin
double roundRobin(vector<Process> processes, int quantum) {
    queue<int> q;
    int n = processes.size();

    vector<int> remaining(n);
    vector<int> arrival(n);
    vector<int> burst(n);
    vector<bool> inQueue(n, false);

    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].burst;
        arrival[i] = processes[i].arrival;
        burst[i] = processes[i].burst;
    }

    int time = 0, completed = 0;
    int total_waiting = 0;

    // Add first arriving processes
    for (int i = 0; i < n; i++) {
        if (arrival[i] == 0) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {
        if (q.empty()) {
            time++;
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && arrival[i] <= time) {
                    q.push(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int i = q.front();
        q.pop();

        int exec_time = min(quantum, remaining[i]);
        remaining[i] -= exec_time;
        time += exec_time;

        // Add newly arrived processes
        for (int j = 0; j < n; j++) {
            if (!inQueue[j] && arrival[j] <= time) {
                q.push(j);
                inQueue[j] = true;
            }
        }

        if (remaining[i] > 0) {
            q.push(i);
        } else {
            completed++;
            int waiting = time - arrival[i] - burst[i];
            total_waiting += waiting;
        }
    }

    return (double)total_waiting / n;
}

int main() {
    vector<Process> processes = readProcesses("processes.txt");

    double fcfs_avg = fcfs(processes);
    double sjf_avg = sjf(processes);
    double rr_avg = roundRobin(processes, 2); // quantum = 2

    cout << "Average Waiting Time:\n";
    cout << "FCFS: " << fcfs_avg << endl;
    cout << "SJF: " << sjf_avg << endl;
    cout << "Round Robin: " << rr_avg << endl;

    ofstream outFile("results.txt");

    outFile << "FCFS " << fcfs_avg << endl;
    outFile << "SJF " << sjf_avg << endl;
    outFile << "RR " << rr_avg << endl;

    outFile.close();

    return 0;
}