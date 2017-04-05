#include "search_statistics.h"

#include "utils/timer.h"
#include "utils/system.h"

#include <iostream>

using namespace std;


SearchStatistics::SearchStatistics() {
    expanded_states = 0;
    reopened_states = 0;
    evaluated_states = 0;
    evaluations = 0;
    generated_states = 0;
    dead_end_states = 0;
    generated_ops = 0;

    lastjump_expanded_states = 0;
    lastjump_reopened_states = 0;
    lastjump_evaluated_states = 0;
    lastjump_generated_states = 0;

    lastjump_f_value = -1;
}

void SearchStatistics::report_f_value_progress(int f) {
    if (f > lastjump_f_value) {
        lastjump_f_value = f;
        print_f_line();
        lastjump_expanded_states = expanded_states;
        lastjump_reopened_states = reopened_states;
        lastjump_evaluated_states = evaluated_states;
        lastjump_generated_states = generated_states;
    }
}

void SearchStatistics::print_f_line() const {
    cout << "f = " << lastjump_f_value
         << " [";
    print_basic_statistics();
    cout << "]" << endl;
}

void SearchStatistics::print_basic_statistics() const {
    cout << evaluated_states << " evaluated, "
         << expanded_states << " expanded, ";
    if (reopened_states > 0) {
        cout << reopened_states << " reopened, ";
    }
    cout << "t=" << Utils::g_timer;
    cout << ", " << Utils::get_peak_memory_in_kb() << " KB";
}

void SearchStatistics::print_detailed_statistics() const {
    cout << "STAT expanded    " << expanded_states  << endl;
    cout << "STAT reopened    " << reopened_states  << endl;
    cout << "STAT evaluated   " << evaluated_states << endl;
    cout << "STAT evaluations " << evaluations      << endl;
    cout << "STAT generated   " << generated_states << endl;
    cout << "STAT dead ends   " << dead_end_states  << endl;

    if (lastjump_f_value >= 0) {
        cout << "STAT expanded_until_last_jump  " << lastjump_expanded_states  << endl;
        cout << "STAT reopened_until_last_jump  " << lastjump_reopened_states  << endl;
        cout << "STAT evaluated_until_last_jump " << lastjump_evaluated_states << endl;
        cout << "STAT generated_until_last_jump " << lastjump_generated_states << endl;
    }
}
