// Lab 3, Daniel Schlatter, CSCI 3761
#include<bits/stdc++.h>
#include <iostream>
#include <chrono>
using namespace std;

typedef  pair<int, int> iPair;                    // shortcut for int pair

/* struct to represent graph  */
struct Graph {
    int V;
    vector< pair<int, iPair> > edges;

    explicit Graph(int V){                        // Constructor
        this->V = V;
    }

    void addEdge(int u, int v, int w){            // funct to add edge
        edges.push_back({w, {u, v}});
    }

    int MST();                                    // Function to find MST using kruskals algorithm
};

/* represent Disjoint Sets */
struct DisjointSets{
    int *parent, *rnk;

    explicit DisjointSets(int n){                 // Constructor.
        parent = new int[n+1];                    // Allocate memory
        rnk = new int[n+1];
        for (int i = 0; i <= n; i++){             // Initially, all vertices are in different sets
            rnk[i] = 0;                           // and have rank 0.
            parent[i] = i;                        // every element is parent of itself
        }
    }

    int find(int u){                              // Find the parent of a node 'u', Path Compression
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u]) parent[u] = find(parent[u]);
        return parent[u];
    }

    void merge(int x, int y){
        x = find(x), y = find(y);
        if (rnk[x] > rnk[y]) parent[y] = x;       // Make tree with smaller height a subtree of the other tree
        else parent[x] = y;
        if (rnk[x] == rnk[y]) rnk[y]++;
    }
};

/* returns weight of the MST */
int Graph::MST() {
    int mst_wt = 0;                                     // will hold cost result
    sort(edges.begin(), edges.end());                   // sort edges by cost
    DisjointSets ds(V);                                 // Create disjoint sets
    vector< pair<int, iPair> >::iterator it;
    vector <vector <pair<int, int>>> printVector;
    for (it=edges.begin(); it!=edges.end(); it++){      // Iterate through all sorted edges
        int x = it->second.first;
        int y = it->second.second;
        int set1 = ds.find(x);
        int set2 = ds.find(y);
        // Check if the selected edge is creating a cycle or not (Cycle is created if x and y belong to same set)
        if (set1 != set2){                              // Current edge will be in the MST
            bool newvect=true;                          // so add to vector to print
            for(int i = 0; i < printVector.size(); i++)
                if ((x==1 && printVector[i][0].first == y) || (printVector[i][printVector[i].size()-1].second == x)){
                    printVector[i].push_back({x, y});
                    sort(printVector[i].begin(), printVector[i].end());
                    newvect=false;
                }
            if(newvect) {
                vector<pair<int,int>> vector1;
                vector1.push_back({x,y});
                printVector.push_back(vector1);
            }
            mst_wt += it->first;                        // Update MST weight
            ds.merge(set1, set2);                       // Merge two sets
        }
    }
    for(int i = 0; i < printVector.size(); i++) {       // print mst
        for(int j =0; j < printVector[i].size(); j++) {
            cout << '(' << printVector[i][j].first << "," << printVector[i][j].second << ")";
            if(j <printVector[i].size() -1) cout << "->";
            else if(i < printVector.size() -1 ) cout << ", ";
        }
    }
    return mst_wt;
}

/* Driver program */
int main() {
    string instruction;
    while (instruction != "exit") {
        cout << "Enter mymst <input-file-name> to get a minimum spanning tree or enter exit to exit the program.\n";
        cin >> instruction;
        transform(instruction.begin(), instruction.end(),
                  instruction.begin(), ::tolower);
        if (instruction == "mymst") {
            ifstream myfile;
            string filename;
            cin >> filename;
            myfile.open(filename);
            if(myfile) {
                int V, i, j, cost;
                myfile >> V;
                Graph g(V);                                 // create graph from file
                while (myfile >> i >> j >> cost) g.addEdge(i, j, cost);
                myfile.close();
                cout << "Minimum Spanning Tree (T): ";
                auto start = chrono::steady_clock::now();   // start clock
                int mst_wt = g.MST();                       // calculate mst
                auto end = chrono::steady_clock::now();     // end clock
                cout << "\nTotal Execution time = "
                     << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                     << " ms" << endl
                     << "Minimum Cost = " << mst_wt << endl;
            }
            else cout << "Pleaser enter a valid filename.\n";
        }
        else if( instruction != "exit") cout << "Please enter a valid command.\n";
    }
    return 0;
}
