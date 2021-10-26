/*
AUTHOR: Matthew Wirasakti
STID: 4160
DATE: December 3, 2020
CLASS: CS315-001 Fall 2020
PROJECT: fp.cpp
COMPILATION: g++ fp.cpp
SYNOPSIS: fp.cpp < input.txt
COMPUTER: VM allocated by the CS department
ALGORITHM: Topological Sorting, Lists, Stacks, Vectors
COMPLEXITY: O(V + E)
BUGS: No issues noticed
REFERENCES: PowerPoint includes all references at the end
NOTES: See README file
*/

#include<string>
#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<stack>
#include<list>

using namespace std;

// constant declared to use in place of infinity
const int MAX = 1000;

// declaration of a single event, or node, with several descriptors and related functions
class Event {
    int to;
    int time;
    int optimistic;
    int pessimistic;
public:
    Event(int a, int b, int c, int d) {
        to = a;
        time = b;
        optimistic = c;
        pessimistic = d;
    }
    int getTo() {
        return to;
    } 
    int getTime() {
        return time;
    }
    int getOpt() {
        return optimistic;
    }
    int getPes() {
        return pessimistic;
    }
};

// declaration of the project
class Project {
    int n;
    list<Event>* adj;
    // function used to topologically sort the events
    void topSort (int u, bool visited[], stack<int> &visitedEvents);
public:
    // constructor used to allocate a specified amount of space in the list
    Project(int n) {
        this->n = n;
        adj = new list<Event>[n];
    }
    // function used to add a new event to the project
    void inputEvent(int from, int to, int time, int pessimistic, int optimistic) {
        Event newEvent(to, time, pessimistic, optimistic);
        adj[from].push_back(newEvent);
    }
    // produces the output of the project, events, paths, and times
    void calculatedOutput(int start, int numEvents); 
};

void Project::topSort (int u, bool visited[], stack<int> &visitedEvents) {
    visited[u] = true;
    for (list<Event>::iterator i = adj[u].begin(); i != adj[u].end(); i++) {
        Event currEvent = *i;
        if (!visited[currEvent.getTo()]) {
            // recursive function used to check each event and to sort
            topSort(currEvent.getTo(), visited, visitedEvents);
        }
    }
    visitedEvents.push(u);
}
    
void Project::calculatedOutput(int start, int numEvents){
    // stacks used to store times
    vector<int> sWeight, lWeight, oWeight, pWeight;
    stack<int> visitedEvents, sortedEvents;

    // initializes all of the events as unvisited
    bool* visited = new bool[numEvents];
    for (int i = 0; i < numEvents; i++) {
        visited[i] = false;
    }
    
    // loops through all the events to sort and count them as visited
    for (int i = 0; i < numEvents; i++) {
        if (visited[i] == false) {
            topSort(i, visited, visitedEvents);
        }
    }
    sortedEvents = visitedEvents;
    
    // loops through each stack of times and places each spot as a MAX time
    for (int i = 0; i < numEvents; i++) {
        sWeight.push_back(MAX);
        lWeight.push_back(MAX);
        oWeight.push_back(MAX);
        pWeight.push_back(MAX);
    }
    // start is used as user input to determine the starting point of the project
    sWeight[start] = 0;
    lWeight[start] = 0;
    oWeight[start] = 0;
    pWeight[start] = 0;

    // goes through every stack and looks at the times given for each event, then calculating the shortest (or longest) time from the starting event to every other event
    while (sortedEvents.empty() == false) {
        int x = sortedEvents.top();
        sortedEvents.pop();
        if (sWeight[x] != MAX) {
            for (list<Event>::iterator i = adj[x].begin(); i != adj[x].end(); i++) {
                if (sWeight[i->getTo()] > sWeight[x] + i->getTime()) {
                    sWeight[i->getTo()] = sWeight[x] + i->getTime();
                }
            }
        }
        if (lWeight[x] != MAX) {
            for (list<Event>::iterator i = adj[x].begin(); i != adj[x].end(); i++) {
                if (lWeight[i->getTo()] > lWeight[x] + ((i->getTime()) * -1)) {
                    lWeight[i->getTo()] = lWeight[x] + ((i->getTime()) * -1);
                }
            }
        }
        if (oWeight[x] != MAX) {
            for (list<Event>::iterator i = adj[x].begin(); i != adj[x].end(); i++) {
                if (oWeight[i->getTo()] > oWeight[x] + i->getOpt()) {
                    oWeight[i->getTo()] = oWeight[x] + i->getOpt();
                }
            }
        }
        if (pWeight[x] != MAX) {
            for (list<Event>::iterator i = adj[x].begin(); i != adj[x].end(); i++) {
                if (pWeight[i->getTo()] > pWeight[x] + i->getPes()) {
                    pWeight[i->getTo()] = pWeight[x] + i->getPes();
                }
            }
        }
    }
    // generates the output for each path based on the category of time
    while (visitedEvents.empty() == false) {
        for (int i = 0; i < numEvents; i++) {
            cout << start << " -> ";
            cout << visitedEvents.top() << "\t\t\t";
            if (sWeight[i] == MAX) {
                cout << "INF";
            }
            else {
                cout << sWeight[i];
            }
            cout << "\t\t\t\t";
            lWeight[i] = lWeight[i] * -1;
            if (lWeight[i] == MAX || lWeight[i] == (MAX * -1)) {
                cout << "INF";
            }
            else {
                cout << lWeight[i];
            }
            cout << "\t\t\t\t";
            if (oWeight[i] == MAX) {
                cout << "INF";
            }
            else {
                cout << oWeight[i];
            }
            cout << "\t\t\t";
            if (pWeight[i] == MAX) {
                cout << "INF";
            }
            else {
                cout << pWeight[i];
            }
            cout << "\t\t\t" << endl;
            visitedEvents.pop();
        }
    }
    cout << endl;
}

int main() {
    int numEvents, numPaths, from, to, time, optimistic, pessimistic, start;
    vector<Event> project;
    // receives user input to accomdate to user needs
    cin >> numEvents;
    cin >> numPaths;
    cin >> start;

    Project p(numEvents);
    // inputs given descriptors of each event
    for (int i = 0; i < numPaths; i++) {
        cin >> from >> to >> time >> optimistic >> pessimistic;
        p.inputEvent(from, to, time, optimistic, pessimistic);
    }
    // generates output
    cout << "Edge" << "\t\t" << "Expected Shortest Time" << "\t\t" << "Expected Longest Time" << "\t\t";
    cout << "Optimistic Time" << "\t\t" << "Pessimistic Time" << endl;
    p.calculatedOutput(start, numEvents);
    
    return 0;
}