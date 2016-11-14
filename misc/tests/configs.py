
def configs_satisficing_ipc():
    return {
        "seq_sat_lama_2011": ["--alias", "seq-sat-lama-2011"],
        "seq_sat_fdss_1": ["--alias", "seq-sat-fdss-1"],
        "seq_sat_fdss_2": ["--alias", "seq-sat-fdss-2"],
    }

def configs_satisficing_extended():
    return {
        "h": [
            "--heuristic", "h=ff()", "--search",
            "eager("
            "tiebreaking([h]),"
            "reopen_closed=true,f_eval=h)"],
        "n1h": [
            "--heuristic", "h=ff()",
            "--heuristic", "n=novelty([],1)",
            "--search",
            "eager("
            "tiebreaking([n,h]),"
            "reopen_closed=true)"],
        "n2h": [
            "--heuristic", "h=ff()",
            "--heuristic", "n=novelty([],2)",
            "--search",
            "eager("
            "tiebreaking([n,h]),"
            "reopen_closed=true)"],
        # "n3h": [
        #     "--heuristic", "h=ff()",
        #     "--heuristic", "n=novelty([],3)",
        #     "--search",
        #     "eager("
        #     "tiebreaking([n,h]),"
        #     "reopen_closed=true)"],
        # "hn1": [
        #     "--heuristic", "h=ff()",
        #     "--heuristic", "n=novelty([h],1)"
        #     "--search",
        #     "eager("
        #     "tiebreaking([h,n]),"
        #     "reopen_closed=true)"],
        # "hn2": [
        #     "--heuristic", "h=ff()",
        #     "--heuristic", "n=novelty([h],2)"
        #     "--search",
        #     "eager("
        #     "tiebreaking([h,n]),"
        #     "reopen_closed=true)"],
        # "hn3": [
        #     "--heuristic", "h=ff()",
        #     "--heuristic", "n=novelty([h],3)"
        #     "--search",
        #     "eager("
        #     "tiebreaking([h,n]),"
        #     "reopen_closed=true)"],
    }


def task_transformation_test_configs():
    return {
        "root_task": [
            "--search", "lazy_greedy(ff())"],
        "root_task_no_transform": [
            "--search", "lazy_greedy(ff(transform=no_transform))"],
        "adapt_costs": [
            "--search", "lazy_greedy(ff(transform=adapt_costs(cost_type=plusone)))"],
        "adapt_adapted_costs": [
            "--search",
            "lazy_greedy(ff(transform=adapt_costs(cost_type=plusone,"
                           "transform=adapt_costs(cost_type=plusone))))"],
    }

def regression_test_configs():
    return {
        "pdb": ["--search", "astar(pdb())"],
    }


def default_configs_optimal(core=True, ipc=True, extended=False):
    configs = {}
    if core:
        configs.update(configs_optimal_core())
    if ipc:
        configs.update(configs_optimal_ipc())
    if extended:
        configs.update(configs_optimal_extended())
    return configs


def default_configs_satisficing(core=True, ipc=True, extended=False):
    configs = {}
    if core:
        configs.update(configs_satisficing_core())
    if ipc:
        configs.update(configs_satisficing_ipc())
    if extended:
        configs.update(configs_satisficing_extended())
    return configs
