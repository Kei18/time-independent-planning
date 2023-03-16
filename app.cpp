#include <regex>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <graph.hpp>
#include <causal_pibt.hpp>
#include <causal_pibt_mapf.hpp>
#include <fsp.hpp>
#include <mcp.hpp>
#include <greedy.hpp>
#include <mapf_random.hpp>
#include <mapf_dp.hpp>
#include <stn.hpp>

/*
 * MAPF_RANDOM: pickup one agent randomly
 * MAPF_DP: emulate MAPF with Delay Probabilities
 */
enum ProblemType { PROBLEM_MAPF_RANDOM,
                   PROBLEM_MAPF_DP, };

struct Param {
  ProblemType problem_type;  // MAPF_RANDOM or MAPF_DP
  std::string field_name;    // map
  int num_agent;             // the number of agents
  int seed;                  // random seed
  int max_activation;        // limit of the number of activation
  bool scen_off;             // use random starts or goals
  float delay_prob;          // used only in MAPF_DP, delay probability
  std::string mapf_plan;     // used with FSP, MCP, CAUSAL_PIBT_MAPF
};

void readParam(const std::string& param_file, Param& param);
void readSetNode(const std::string& s, Config& config, Graph* G);
void readMAPFPlan(const std::string& _file, Configs& configs, Graph* G);
std::string strParam(Param& param);
void printHelp();


int main(int argc, char *argv[])
{
  std::string instance_file = "";
  std::string output_file = "result.txt";
  std::string solver_name = "CAUSAL_PIBT";
  bool verbose = false;
  bool log_simple = false;

  struct option longopts[] =
    {
     { "instance", required_argument, 0, 'i' },
     { "output", required_argument, 0, 'o' },
     { "solver", required_argument, 0, 's' },
     { "help", no_argument, 0, 'h' },
     { "seed", no_argument, 0, 'r' },
     { "verbose", no_argument, 0, 'v' },
     { "log-simple", no_argument, 0, 'l' },
     { 0, 0, 0, 0 },
    };

  // command line args
  int opt, longindex;
  int seed = -1;
  opterr = 0;  // ignore getopt error
  while ((opt = getopt_long(argc, argv, "i:o:s:r:hvl",
                            longopts, &longindex)) != -1) {
    switch (opt) {
    case 'i':
      instance_file = std::string(optarg);
      break;
    case 'o':
      output_file = std::string(optarg);
      break;
    case 's':
      solver_name = std::string(optarg);
      break;
    case 'v':
      verbose = true;
      break;
    case 'l':
      log_simple = true;
      break;
    case 'r':
      seed = std::atoi(optarg);
      break;
    case 'h':
      printHelp();
      return 0;
    default:
      break;
    }
  }

  if (instance_file.length() == 0) {
    std::cout << "specify instance file using -i [FILE-NAME], e.g.,"
              << std::endl;
    std::cout << "> ./app -i ../instance/sample.txt" << std::endl;
    return 0;
  }

  // default value
  Param param =
    {
     PROBLEM_MAPF_RANDOM,  // problem_type,
     "3x3.map",            // map_name
     3,                    // num_agent
     0,                    // seed
     1000,                 // max_activation
     true,                 // scen_off
     0.1,                  // delay probability
     "",                   // mapf_plan
    };
  readParam(instance_file, param);
  if (seed != -1) param.seed = seed;


  // initialization
  std::mt19937* MT = new std::mt19937(param.seed);
  Agent::setMT(MT);
  Agent::setVerbose(verbose);

  // make grid
  Graph* G = new Grid(param.field_name);
  Agent::setG(G);

  // read mapf plan
  Configs configs;
  if (param.mapf_plan != "") {
    readMAPFPlan(param.mapf_plan, configs, G);
    // simple check
    if (configs[0].size() != param.num_agent) halt("invalid MAPF plan");
  }

  // make agents
  Agents A;
  for (int i = 0; i < param.num_agent; ++i) {
    Path path;
    for (auto c : configs) path.push_back(c[i]);
    if (solver_name == "CAUSAL_PIBT") {
      A.push_back(new CausalPIBT());
    } else if (solver_name == "GREEDY") {
      A.push_back(new Greedy());
    } else if (solver_name == "CAUSAL_PIBT_MAPF") {
      if (path.empty()) halt("check mapf_plan");
      A.push_back(new CausalPIBT_MAPF(path));
    } else if (solver_name == "FSP") {
      if (path.empty()) halt("check mapf_plan");
      A.push_back(new FSP(path));
    } else if (solver_name == "MCP") {
      if (path.empty()) halt("check mapf_plan");
      A.push_back(new MCP(path));
    } else {
      halt("main", "unknown solver type");
    }
  }

  // make problem
  Problem* P = nullptr;
  switch (param.problem_type) {
  case PROBLEM_MAPF_RANDOM:
    if (param.scen_off) {
      P = new MAPF_RANDOM(G, A, MT, param.max_activation);
    } else {
      P = new MAPF_RANDOM(instance_file, G, A, MT, param.max_activation);
    }
    break;
  case PROBLEM_MAPF_DP:
    if (param.scen_off) {
      P = new MAPF_DP(G, A, MT,
                      param.delay_prob,
                      param.max_activation);
    } else {
      P = new MAPF_DP(instance_file, G, A, MT,
                      param.delay_prob,
                      param.max_activation);
    }
    break;
  default:
    halt("main", "unknown problem type");
    break;
  }

  // emulate
  P->run();

  // output result
  std::string result;
  result += "instance=" + instance_file + "\n";
  result += "agents=" + std::to_string(param.num_agent) + "\n";
  result += "map_file=" + param.field_name + "\n";
  result += "solver=" + solver_name + "\n";
  result += "seed=" + std::to_string(param.seed) + "\n";
  result += "max_activation=" + std::to_string(param.max_activation) + "\n";
  result += "cnt_activation=" + std::to_string(Agent::getCntActivation()) + "\n";
  if (param.mapf_plan != "")
    result += "mapf_plan=" + param.mapf_plan + "\n";
  result += P->strProblem();
  if (!log_simple) result += Agent::strAgent();
  std::ofstream log;
  log.open(output_file, std::ios::out);
  if (!log) halt("main", output_file + " cannot open");
  log << result;
  log.close();

  // for memory management
  for (auto a : A) delete a;
  delete P;
  delete G;
  delete MT;

  return 0;
}


// read & set params
void readParam(const std::string& param_file, Param& param)
{
  std::ifstream file(param_file);
  if (!file) halt("readParam", param_file + " cannot open");

  std::regex r_problem_type = std::regex(R"(problem_type=(.+))");
  std::regex r_solver_type = std::regex(R"(solver_type=(.+))");
  std::regex r_field = std::regex(R"(map_file=(.+))");
  std::regex r_num_agent = std::regex(R"(agents=(\d+))");
  std::regex r_seed = std::regex(R"(seed=(\d+))");
  std::regex r_max_activation = std::regex(R"(max_activation=(\d+))");
  std::regex r_scen_off = std::regex(R"(random_problem=(\d+))");
  std::regex r_delay_prob = std::regex(R"(delay_prob=(\d+(?:\.\d+)?))");
  std::regex r_mapf_plan = std::regex(R"(mapf_plan=(.+))");

  std::string line, tmpstr;
  std::smatch results;

  while (getline(file, line)) {
    if (std::regex_match(line, results, r_problem_type)) {
      tmpstr = results[1].str();
      if (tmpstr == "MAPF_RANDOM") {
        param.problem_type = PROBLEM_MAPF_RANDOM;
      } else if (tmpstr == "MAPF_DP") {
        param.problem_type = PROBLEM_MAPF_DP;
      } else {
        warn( results[1].str() + " is unknown");
      }
    } else if (std::regex_match(line, results, r_field)) {
      param.field_name = results[1].str();
    } else if (std::regex_match(line, results, r_num_agent)) {
      param.num_agent = std::stoi(results[1].str());
    } else if (std::regex_match(line, results, r_seed)) {
      param.seed = std::stoi(results[1].str());
    } else if (std::regex_match(line, results, r_max_activation)) {
      param.max_activation = std::stoi(results[1].str());
    } else if (std::regex_match(line, results, r_scen_off)) {
      param.scen_off = (bool)std::stoi(results[1].str());
    } else if (std::regex_match(line, results, r_delay_prob)) {
      param.delay_prob = std::stof(results[1].str());
    } else if (std::regex_match(line, results, r_mapf_plan)) {
      param.mapf_plan = results[1].str();
    }
  }
}

// read MAPF plan
void readMAPFPlan(const std::string& plan_file, Configs& configs, Graph* G)
{
  std::ifstream file(_PLANDIR_ + plan_file);
  if (!file) halt("readParam", plan_file + " cannot open");

  int num_agent;

  std::regex r_num_agent = std::regex(R"(agents=(\d+))");
  std::regex r_sol       = std::regex(R"(solution=)");
  std::regex r_config    = std::regex(R"(\d+:(.+))");

  std::string line;
  std::smatch results;
  while (getline(file, line)) {
    if (std::regex_match(line, results, r_num_agent)) {
      num_agent = std::stoi(results[1].str());
      continue;
    }
    // solution
    if (std::regex_match(line, results, r_sol)) {
      while (getline(file, line)) {
        if (std::regex_match(line, results, r_config)) {
          Config c;
          readSetNode(results[1].str(), c, G);
          configs.push_back(c);
        }
      }
    }
  }
}

void readSetNode(const std::string& s, Config& config, Graph* G)
{
  if (G == nullptr) halt("graph is not read.");
  std::regex r_pos = std::regex(R"(\((\d+),(\d+)\),)");
  std::smatch m;
  auto iter = s.cbegin();
  while (std::regex_search(iter, s.cend(), m, r_pos)) {
    iter = m[0].second;
    int x = std::stoi(m[1].str());
    int y = std::stoi(m[2].str());
    if (!G->existNode(x, y))
      halt("node does not exist");
    config.push_back(G->getNode(x, y));
  }
}

void printHelp()
{
  std::cout << "\nUsage: ./app [OPTIONS]\n"
            << "  -i --instance [FILE_PATH]     instance file path\n"
            << "  -o --output [FILE_PATH]       ouptut file path\n"
            << "  -s --solver [SOLVER_NAME]     solver, choose from "
            << "[ GREEDY, CAUSAL_PIBT, CAUSAL_PIBT_MAPF, FSP, MCP ]\n"
            << "  -l --log-simple               use simple log\n"
            << "  -v --verbose                  print additional info\n"
            << "  -h --help                     help\n"
            << std::endl;
}
