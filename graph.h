#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <unordered_map>
#include <set>
#include <iostream>
#include <algorithm>
#include <queue>
#include <limits>
#include <random>

class Graph {
public:
    Graph(int numUsers);

    void addConnection(int user1, int user2);
    void generateRandomConnections(int numConnections);

    void displayUsers();
    void displayNetworkStats();

    std::vector<int> friendSuggestions(int user);
    int degreeOfSeparation(int user1, int user2);
    std::vector<std::vector<int>> stronglyConnectedComponents();
    std::vector<int> influentialUsers();

    std::vector<int> topUsersByConnections(int topN);

private:
    int numUsers;
    std::vector<std::list<int>> adjList;

    void dfs(int user, std::vector<bool>& visited, std::vector<int>& component);
};

#endif
