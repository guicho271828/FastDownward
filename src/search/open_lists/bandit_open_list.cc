
#include "bandit_open_list.h"

// #include "../debug.h"

#include <cassert>
#include <memory>
#include <vector>

using namespace bandit;
using namespace std;

Options bandit::add_bandit_option_to_parser(OptionParser &parser, string def_type) {
    vector<string> reward_types;
    vector<string> reward_types_doc;
    reward_types.push_back("NORMAL");
    reward_types_doc.push_back("A lever gets a reward equal to the absolute difference to the h value.");
    reward_types.push_back("RELATIVE");
    reward_types_doc.push_back("A lever gets a reward equal to the improvement of h relative to the initial heuristic value (0.0-1.0).");
    reward_types.push_back("ONE");
    reward_types_doc.push_back("A lever always gets a reward of 1.0.");
    parser.add_enum_option("reward_type", reward_types,
                           "Specifies the amount of reward when h value is improved.",
                           def_type, reward_types_doc);
    parser.add_option<bool>("pull_once", "count pulls only once per dequeing", "true");
    Options opts = parser.parse();
    opts.verify_list_non_empty<ScalarEvaluator *>("evals");
    return opts;
}
