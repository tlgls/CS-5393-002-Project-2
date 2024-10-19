#include "graph.h"
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()

int main() {
    const int numUsers = 250;
    const int numConnections = 500;  // adjust this for realistic density

    Graph network(numUsers);

    // generates random connections for the network
    network.generateRandomConnections(numConnections);

    // displays users and their connections
    std::cout << "User Connections:" << std::endl;
    network.displayUsers();

    // displays basic network statistics
    std::cout << "\nNetwork Statistics:" << std::endl;
    network.displayNetworkStats();

    // shows friend suggestions for a user (example: User 0)
    std::cout << "\nFriend Suggestions for User 0:" << std::endl;
    auto suggestions = network.friendSuggestions(0);
    for (int s : suggestions) {
        std::cout << "User " << s << std::endl;
    }

    // calculates and display degree of separation for 5 random pairs
    std::srand(std::time(0));  // seed for randomness
    std::cout << "\nDegree of Separation for 5 Random Pairs:" << std::endl;

    for (int i = 0; i < 5; ++i) {
        int user1 = std::rand() % numUsers;
        int user2 = std::rand() % numUsers;
        while (user1 == user2) {
            user2 = std::rand() % numUsers;  // ensures the two users are different
        }
        int separation = network.degreeOfSeparation(user1, user2);
        std::cout << "Between User " << user1 << " and User " << user2 << ": ";
        if (separation != -1) {
            std::cout << separation << " hops" << std::endl;
        } else {
            std::cout << "No path exists" << std::endl;
        }
    }

    // identifies strongly connected components
    std::cout << "\nStrongly Connected Components (Top 5):" << std::endl;
    auto components = network.stronglyConnectedComponents();
    for (int i = 0; i < std::min(5, static_cast<int>(components.size())); ++i) {
        std::cout << "Component " << i + 1 << ": ";
        for (int user : components[i]) {
            std::cout << user << " ";
        }
        std::cout << std::endl;
    }

    // analyzes influential users
    std::cout << "\nTop 5 Influential Users:" << std::endl;
    auto influential = network.influentialUsers();
    for (int user : influential) {
        std::cout << "User " << user << std::endl;
    }

    return 0;
}

