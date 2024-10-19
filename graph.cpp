#include "graph.h"

// constructor to initialize graph with given number of users (nodes)
Graph::Graph(int numUsers) : numUsers(numUsers), adjList(numUsers) {}

// add connection (edge) between two users in graph
void Graph::addConnection(int user1, int user2) {
    adjList[user1].push_back(user2);
    adjList[user2].push_back(user1);
}

// generates realistic random connections between users in graph
void Graph::generateRandomConnections(int numConnections) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, numUsers - 1);

    for (int i = 0; i < numConnections; ++i) {
        int user1 = dist(gen);
        int user2 = dist(gen);
        if (user1 != user2) addConnection(user1, user2);
    }
}

// displays each user and their connections
void Graph::displayUsers() {
    for (int i = 0; i < numUsers;
         ++i) {
        std::cout << "User " << i << ": ";
        for (int conn : adjList[i]) {
            std::cout << conn << " ";
        }
        std::cout << std::endl;
    }
}

// displays basic network statistics
void Graph::displayNetworkStats() {
    int totalConnections = 0;
    for (const auto& connections : adjList) {
        totalConnections += connections.size();
    }
    std::cout << "Number of Users: " << numUsers << std::endl;
    std::cout << "Average Connections per User: "
              << static_cast<double>(totalConnections) / numUsers << std::endl;

    std::vector<int> topUsers = topUsersByConnections(10);
    std::cout << "Top 10 Users with Max Connections:" << std::endl;
    for (int user : topUsers) {
        std::cout << "User " << user << ": " << adjList[user].size() << " connections" << std::endl;
    }
}

//suggests friends based on mutual connections
std::vector<int> Graph::friendSuggestions(int user) {
    std::unordered_map<int, int> mutualFriends;

    for (int friend1 : adjList[user]) {
        for (int friend2 : adjList[friend1]) {
            if (friend2 != user &&
                std::find(adjList[user].begin(), adjList[user].end(), friend2) == adjList[user].end()) {
                mutualFriends[friend2]++;
            }
        }
    }

    std::vector<std::pair<int, int>> sortedSuggestions(mutualFriends.begin(), mutualFriends.end());
    std::sort(sortedSuggestions.begin(), sortedSuggestions.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    std::vector<int> suggestions;
    for (int i = 0; i < std::min(5, static_cast<int>(sortedSuggestions.size())); ++i) {
        suggestions.push_back(sortedSuggestions[i].first);
    }

    return suggestions;
}

// finds degree of separation (shortest path) using bfs
int Graph::degreeOfSeparation(int user1, int user2) {
    std::vector<int> dist(numUsers, std::numeric_limits<int>::max());
    std::queue<int> q;
    q.push(user1);
    dist[user1] = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adjList[u]) {
            if (dist[v] == std::numeric_limits<int>::max()) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist[user2] == std::numeric_limits<int>::max() ? -1 : dist[user2];
}

// helper function for dfs to find strongly connected components
void Graph::dfs(int user, std::vector<bool>& visited, std::vector<int>& component) {
    visited[user] = true;
    component.push_back(user);
    for (int neighbor : adjList[user]) {
        if (!visited[neighbor]) dfs(neighbor, visited, component);
    }
}

// identifies strongly connected components
std::vector<std::vector<int>> Graph::stronglyConnectedComponents() {
    std::vector<bool> visited(numUsers, false);
    std::vector<std::vector<int>> components;

    for (int i = 0; i < numUsers; ++i) {
        if (!visited[i]) {
            std::vector<int> component;
            dfs(i, visited, component);
            components.push_back(component);
        }
    }

    return components;
}

// finds the top N users with most connections
std::vector<int> Graph::topUsersByConnections(int topN) {
    std::vector<std::pair<int, int>> userConnections;
    for (int i = 0; i < numUsers; ++i) {
        userConnections.emplace_back(i, adjList[i].size());
    }

    std::sort(userConnections.begin(), userConnections.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    std::vector<int> topUsers;
    for (int i = 0; i < std::min(topN, static_cast<int>(userConnections.size())); ++i) {
        topUsers.push_back(userConnections[i].first);
    }

    return topUsers;
}

// analyzes influential users based on degree centrality (top 5)
std::vector<int> Graph::influentialUsers() {
    return topUsersByConnections(5);
}
