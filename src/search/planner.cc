#include "option_parser.h"
#include "search_engine.h"

#include "utils/timer.h"
#include "utils/system.h"
#include "globals.h"

#include <iostream>

using namespace std;
using Utils::ExitCode;

int main(int argc, const char **argv) {
    Utils::register_event_handlers();

    if (argc < 2) {
        cout << OptionParser::usage(argv[0]) << endl;
        Utils::exit_with(ExitCode::INPUT_ERROR);
    }

    if (string(argv[1]).compare("--help") != 0)
        read_everything(cin);

    // The command line is parsed twice: once in dry-run mode, to
    // check for simple input errors, and then in normal mode.
    bool unit_cost = is_unit_cost();
    try {
        OptionParser::parse_cmd_line(argc, argv, true, unit_cost);
        g_engine = OptionParser::parse_cmd_line(argc, argv, false, unit_cost);
    } catch (ArgError &error) {
        cerr << error << endl;
        OptionParser::usage(argv[0]);
        Utils::exit_with(ExitCode::INPUT_ERROR);
    } catch (ParseError &error) {
        cerr << error << endl;
        Utils::exit_with(ExitCode::INPUT_ERROR);
    }

    Utils::Timer search_timer;
    g_engine->search();
    search_timer.stop();
    Utils::g_timer.stop();

    g_engine->save_plan_if_necessary();
    g_engine->print_statistics();
    cout << "Search time: " << search_timer << endl;
    cout << "Total time: " << Utils::g_timer << endl;
    cout << " <INFO> [00:00:00] fastdownward (planner.cc) - [0.000s] [+0.000s] eval/sec: " << g_engine->get_statistics().get_evaluated_states() / search_timer() << endl;

    if (g_engine->found_solution()) {
        Utils::exit_with(ExitCode::PLAN_FOUND);
    } else {
        Utils::exit_with(ExitCode::UNSOLVED_INCOMPLETE);
    }
}
