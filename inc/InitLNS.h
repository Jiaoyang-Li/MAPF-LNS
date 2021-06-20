#pragma once
#include "BasicLNS.h"

enum init_destroy_heuristic { TARGET_BASED, COLLISION_BASED, RANDOM_BASED, INIT_COUNT };

class InitLNS : public BasicLNS
{
public:
    vector<Agent>& agents;
    int num_of_colliding_pairs = 0;

    InitLNS(const Instance& instance, vector<Agent>& agents, const string & replan_algo_name,
            const string & init_destory_name, int neighbor_size, int screen);

    bool getInitialSolution();
    bool run(double _time_limit);
    void writeIterStatsToFile(const string & file_name) const;
    void writeResultToFile(const string & file_name, int sum_of_distances, double preprocessing_time) const;
    string getSolverName() const override { return "InitLNS(" + replan_algo_name + ")"; }

    void printPath() const;
    void printResult();
    void clear(); // delete useless data to save memory

    void updatePaths(const list<int>& delayed_agents, int timestep, double time_limit);
    void considerFollowingCollisions() { path_table.ignore_following_collisions = false; }
private:
    string replan_algo_name;
    init_destroy_heuristic init_destroy_strategy = COLLISION_BASED;
    vector<Path*> paths; // pointers to paths of all agents
    PathTableWC path_table; // 1. stores the paths of all agents in a time-space table;
    // 2. avoid making copies of this variable as much as possible.

    vector<set<int>> collision_graph;
    vector<int> goal_table;

    bool runPP(int timestep = 0);
    bool runGCBS();
    bool runPBS();

    bool updateCollidingPairs(set<pair<int, int>>& colliding_pairs, int agent_id, const Path& path) const;

    void chooseDestroyHeuristicbyALNS();

    bool generateNeighbor();
    bool generateNeighborByCollisionGraph();
    bool generateNeighborByTarget();
    bool generateNeighborRandomly();

    // int findRandomAgent() const;
    int randomWalk(int agent_id);

    void printCollisionGraph() const;

    static unordered_map<int, set<int>>& findConnectedComponent(const vector<set<int>>& graph, int vertex,
            unordered_map<int, set<int>>& sub_graph);

    bool validatePathTable() const;

    void saveNeighborInformation();
    void updateCollisionGraph();
    void updateDestroyWeights();
    void saveIterationInformation();
};
