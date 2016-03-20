#include "open_list.h"

#include "../option_parser.h"
#include <vector>
using namespace std;

void add_queue_type_option_to_parser(OptionParser &parser, string def_type) {
    vector<string> queue_types;
    vector<string> queue_types_doc;
    queue_types.push_back("FIFO");
    queue_types_doc.push_back("");
    queue_types.push_back("LIFO");
    queue_types_doc.push_back("");
    queue_types.push_back("RANDOM");
    queue_types_doc.push_back("");
    parser.add_enum_option("queue_type", queue_types,
                           "Enqueue/dequeue order when all evals returned the same value.",
                           def_type, queue_types_doc);
}
