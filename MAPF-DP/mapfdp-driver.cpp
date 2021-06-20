#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include "LNS.h"
#include <regex>


void readMAPFPlan(const string& plan_file, vector<Agent>& agents, const Instance & instance)
{
    std::ifstream file(plan_file);
    if (!file)
    {
        cerr << plan_file << " cannot open" << endl;
        exit(-1);
    }

    std::regex r_num_agent = std::regex(R"(agents=(\d+))");
    std::regex r_sol       = std::regex(R"(solution=)");
    std::regex r_config    = std::regex(R"(\d+:(.+))");

    std::string line;
    std::smatch results;
    while (getline(file, line)) {
        if (std::regex_match(line, results, r_num_agent)) {
            int num_agent = std::stoi(results[1].str());
            assert(num_agent == (int)agents.size());
            continue;
        }
        // solution
        if (std::regex_match(line, results, r_sol)) {
            int i = 0;
            while (getline(file, line)) {
                if (std::regex_match(line, results, r_config)) {
                    auto s = results[1].str();
                    std::regex r_pos = std::regex(R"(\((\d+),(\d+)\),)");
                    std::smatch m;
                    auto iter = s.cbegin();
                    while (std::regex_search(iter, s.cend(), m, r_pos)) {
                        iter = m[0].second;
                        int x = std::stoi(m[1].str());
                        int y = std::stoi(m[2].str());
                        agents[i].path.emplace_back(instance.linearizeCoordinate(y, x));
                    }
                    assert(agents[i].path.front().location == agents[i].path_planner->start_location);
                    assert(agents[i].path.back().location == agents[i].path_planner->goal_location);
                    i++;
                }
            }
        }
    }
}

/* Main function */
int main(int argc, char** argv)
{
	namespace po = boost::program_options;
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")

		// params for the input instance and experiment settings
		("map,m", po::value<string>()->required(), "input file for map")
		("agents,a", po::value<string>()->required(), "input file for agents")
		("agentNum,k", po::value<int>()->default_value(0), "number of agents")
		("mapfPlan,p", po::value<string>(), "int file for MAPF plan")
        ("output,o", po::value<string>(), "output file name (no extension)")
		("cutoffTime,t", po::value<double>()->default_value(7200), "cutoff time (seconds)")
		("screen,s", po::value<int>()->default_value(0),
		        "screen option (0: none; 1: Simulation results; 2: LNS results; 3:LNS detailed results; 4: MAPF detailed results)")
		("stats", po::value<string>(), "output stats file")

		// params for the MAPF-DP model
		("delayProbability", po::value<double>()->default_value(0.1), "max delay probability")
        // params for LNS
        ("sipp", po::value<bool>()->default_value(true), "Use SIPP as the single-agent solver")
        ("neighborSize", po::value<int>()->default_value(8), "Size of the neighborhood")
        ("maxIterations", po::value<int>()->default_value(1000000), "maximum number of iterations")
        ("destoryStrategy", po::value<string>()->default_value("Adaptive"),
                "Heuristics for finding subgroups by LNS (Random, RandomWalk, Intersection, Adaptive)")
         ("initDestoryStrategy", po::value<string>()->default_value("Adaptive"),
          "Heuristics for finding subgroups by initLNS (Target, Collision, Random, Adaptive)")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")) {
		cout << desc << endl;
		return 1;
	}

    PIBTPPS_option pipp_option;
    pipp_option.windowSize = 5;
    pipp_option.winPIBTSoft = true;

    po::notify(vm);

	srand((int)time(0));

	Instance instance(vm["map"].as<string>(), vm["agents"].as<string>(),
		vm["agentNum"].as<int>());
    instance.ignore_following_collisions = false;
    double time_limit = vm["cutoffTime"].as<double>();
    int screen = vm["screen"].as<int>();
	srand(0);

    LNS lns(instance, time_limit,
            "PP",
            "PP",
            vm["destoryStrategy"].as<string>(),
            vm["neighborSize"].as<int>(),
            vm["maxIterations"].as<int>(),
            true,
            vm["initDestoryStrategy"].as<string>(),
            vm["sipp"].as<bool>(),
            screen - 1, pipp_option);

    if (vm.count("mapfPlan"))
    {
        readMAPFPlan(vm["mapfPlan"].as<string>(), lns.agents, instance);
    }
    // solve the instance
    /*bool succ = lns.run();
    if (succ)
        lns.validateSolution();
    if (vm.count("output"))
        lns.writeResultToFile(vm["output"].as<string>());
    if (vm.count("stats"))
        lns.writeIterStatsToFile(vm["stats"].as<string>());
    lns.writePathsToFile("path.txt");*/

    /* -----------------execute the instance------------------- */
    // initialize delay probability
    auto max_p = vm["delayProbability"].as<double>();
    vector<double> delay_prob(lns.agents.size());
    for (double & p : delay_prob)
        p = (rand() % RAND_MAX) * max_p;

    // initialize initLNS
    InitLNS init_lns(instance, lns.agents, "PP", vm["initDestoryStrategy"].as<string>(),
            vm["neighborSize"].as<int>(), screen - 1);
    init_lns.considerFollowingCollisions();
    init_lns.run(time_limit); // resolve collisions if the initial solution has one

    // put agents on the map
    vector<int> occupied(instance.map_size, -1);
    if (screen > 0)
        cout << "t=" << 0 << ":\t";
    for (const auto & agent : lns.agents)
    {
        occupied[agent.path_planner->start_location] = agent.id;
        cout << agent.path.front().location << "\t";
    }

    // start simulation
    size_t num_at_goal = 0; // the number of agents that are at their goal locations
    for (auto t = 0; num_at_goal < lns.agents.size(); t++)
    {
        if (screen > 0)
            cout << endl << "t=" << t+1 << ":\t";
        list<int> delayed_agents;
        num_at_goal = 0;
        for (const auto & agent : lns.agents)
        {
            if (t >= agent.path.size() - 1) // the agent has already reached its goal
            {
                assert(occupied[agent.path.back().location] == agent.id);
                assert(agent.path.back().location == agent.path_planner->goal_location);
                num_at_goal++;
                if (screen > 0)
                    cout << "-\t";
            }
            else if (agent.path[t].location == agent.path[t+1].location) // wait agent
            {
                assert(occupied[agent.path[t].location] == agent.id);
                if (screen > 0)
                    cout << agent.path[t+1].location << "\t";
            }
            else if (rand() % RAND_MAX < delay_prob[agent.id]) // the agent gets delayed
            {
                delayed_agents.push_back(agent.id);
                if (screen > 0)
                    cout << "@\t";
            }
            else // move forward one step
            {
                assert(occupied[agent.path[t].location] == agent.id);
                occupied[agent.path[t].location] = -1;
                assert(occupied[agent.path[t+1].location] == -1);
                occupied[agent.path[t+1].location] = agent.id;
                if (screen > 0)
                    cout << agent.path[t+1].location << "\t";
            }
        }
        init_lns.updatePaths(delayed_agents, t, time_limit);
    }
    if (screen > 0)
        cout << endl;
    int sum_of_costs = 0;
    int makespan = 0;
    for (const auto & agent : lns.agents)
    {
        sum_of_costs += (int)agent.path.size() - 1;
        makespan = max(makespan, (int)agent.path.size() - 1);
    }
    cout << "Makespan = " << makespan << endl;
    cout << "Sum of costs = " << sum_of_costs << endl;
	return 0;

}