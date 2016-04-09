def configs_optimal_core():
    return {
        # A*
        "help": ["--help"],
        "astar_blind": [
            "--search",
            "astar(blind)"],
        "astar_h2": [
            "--search",
            "astar(hm(2))"],
        "astar_ipdb": [
            "--search",
            "astar(ipdb)"],
        "astar_lmcount_lm_merged_rhw_hm": [
            "--search",
            "astar(lmcount(lm_merged([lm_rhw(),lm_hm(m=1)]),admissible=true),mpd=true)"],
        "astar_lmcut": [
            "--search",
            "astar(lmcut)"],
        "astar_hmax": [
            "--search",
            "astar(hmax)"],
        "astar_merge_and_shrink_rl_fh": [
            "--search",
            "astar(merge_and_shrink("
            "merge_strategy=merge_linear(variable_order=reverse_level),"
            "shrink_strategy=shrink_fh(max_states=50000),"
            "label_reduction=exact(before_shrinking=false,"
            "before_merging=true)))"],
        "astar_merge_and_shrink_dfp_bisim": [
            "--search",
            "astar(merge_and_shrink(merge_strategy=merge_dfp,"
            "shrink_strategy=shrink_bisimulation(max_states=50000,threshold=1,"
            "greedy=false),"
            "label_reduction=exact(before_shrinking=true,"
            "before_merging=false)))"],
        "astar_merge_and_shrink_dfp_greedy_bisim": [
            "--search",
            "astar(merge_and_shrink(merge_strategy=merge_dfp,"
            "shrink_strategy=shrink_bisimulation(max_states=infinity,threshold=1,"
            "greedy=true),"
            "label_reduction=exact(before_shrinking=true,"
            "before_merging=false)))"],
    }

MERGE_AND_SHRINK = ('astar(merge_and_shrink('
    'merge_strategy=merge_dfp,'
        'shrink_strategy=shrink_bisimulation('
         'max_states=50000,'
        'threshold=1,'
        'greedy=false),'
    'label_reduction=exact('
        'before_shrinking=true,'
        'before_merging=false)'
'))')


def configs_satisficing_core():
    return {
        # A*
        "astar_goalcount": [
            "--search",
            "astar(goalcount)"],
        # eager greedy
        "eager_greedy_ff": [
            "--heuristic",
            "h=ff()",
            "--search",
            "eager_greedy(h, preferred=h)"],
        "eager_greedy_add": [
            "--heuristic",
            "h=add()",
            "--search",
            "eager_greedy(h, preferred=h)"],
        "eager_greedy_cg": [
            "--heuristic",
            "h=cg()",
            "--search",
            "eager_greedy(h, preferred=h)"],
        "eager_greedy_cea": [
            "--heuristic",
            "h=cea()",
            "--search",
            "eager_greedy(h, preferred=h)"],
        # lazy greedy
        "lazy_greedy_ff": [
            "--heuristic",
            "h=ff()",
            "--search",
            "lazy_greedy(h, preferred=h)"],
        "lazy_greedy_add": [
            "--heuristic",
            "h=add()",
            "--search",
            "lazy_greedy(h, preferred=h)"],
        "lazy_greedy_cg": [
            "--heuristic",
            "h=cg()",
            "--search",
            "lazy_greedy(h, preferred=h)"],
    }


def configs_optimal_ipc():
    return {
        "seq_opt_merge_and_shrink": ["--alias", "seq-opt-merge-and-shrink"],
        "seq_opt_fdss_1": ["--alias", "seq-opt-fdss-1"],
        "seq_opt_fdss_2": ["--alias", "seq-opt-fdss-2"],
    }


def configs_satisficing_ipc():
    return {
        "seq_sat_lama_2011": ["--alias", "seq-sat-lama-2011"],
        "seq_sat_fdss_1": ["--alias", "seq-sat-fdss-1"],
        "seq_sat_fdss_2": ["--alias", "seq-sat-fdss-2"],
    }


def configs_optimal_extended():
    return {
        # A*
        "astar_lmcount_lm_merged_rhw_hm_no_order": [
            "--search",
            "astar(lmcount(lm_merged([lm_rhw(),lm_hm(m=1)]),admissible=true),mpd=true)"],
        # different last-resort
        "tiebreaking_lmcut": [
            "--heuristic", "h=lmcut()",
            "--search", "eager(tiebreaking([sum([g(), h]), h]), reopen_closed=true,"
            "f_eval=sum([g(), h]))"],
        "tiebreaking_lmcut_FIFO": [
            "--heuristic", "h=lmcut()",
            "--search", "eager(tiebreaking([sum([g(), h]), h],queue_type=FIFO), reopen_closed=true,"
            "f_eval=sum([g(), h]))"],
        "tiebreaking_lmcut_RANDOM": [
            "--heuristic", "h=lmcut()",
            "--search", "eager(tiebreaking([sum([g(), h]), h],queue_type=RANDOM), reopen_closed=true,"
            "f_eval=sum([g(), h]))"],
        "tiebreaking_lmcut_LIFO": [
            "--heuristic", "h=lmcut()",
            "--search", "eager(tiebreaking([sum([g(), h]), h],queue_type=LIFO), reopen_closed=true,"
            "f_eval=sum([g(), h]))"],
        "typed_tiebreaking_lmcut_RandomDepth_RandomOrder": [
            "--heuristic", "h=lmcut()",
            "--search", "eager(typed_tiebreaking([sum([g(), h]), h],[depth([sum([g(), h]), h])],queue_type=RANDOM),"
            " reopen_closed=true,"
            "f_eval=sum([g(), h]))"],
        "typed_tiebreaking_lmcut_RandomDepth_RandomOrder": [
            "--heuristic", "h=lmcut()",
            "--search", "eager(typed_tiebreaking([sum([g(), h]), h],[depth([sum([g(), h]), h])],queue_type=RANDOM,stochastic=false),"
            " reopen_closed=true,"
            "f_eval=sum([g(), h]))"],
    }

SYN = "hlm,hff=lm_ff_syn(lm_rhw(reasonable_orders=true,lm_cost_type=one,cost_type=one))"
    
def configs_satisficing_extended():
    return {
        # "--cea":    ["--search", "lazy(single(cea()))"],
        "--ff":     ["--search", "lazy(single(ff()))"],
        # "--lmcount":    ["--search", "lazy(single(lmcount(lm_rhw(reasonable_orders=true))))"],
        "lama":     ["--heuristic", SYN,
                     "--search",
                     "lazy(alt(["
                     "single(hff),single(hff,pref_only=true),"
                     "single(hlm),single(hlm,pref_only=true)]),"
                     "preferred=[hff,hlm])"],
        "eg0.2":     ["--heuristic",SYN,
                      "--search", "lazy(eg([hff,hlm],epsilon=0.2))"],
        "ucb2.0":    ["--heuristic",SYN,
                      "--search", "lazy(ucb([hff,hlm],k=2.0))"],
        "ed0.1":     ["--heuristic",SYN,
                      "--search", "lazy(ed([hff,hlm],decay=0.1))"],
        "ed0.001":   ["--heuristic",SYN,
                      "--search", "lazy(ed([hff,hlm],decay=0.001))"],
        # 
        "alt":       ["--heuristic",SYN,
                      "--search", "lazy(alt([single(hff),single(hlm)]))"],
        "pareto":    ["--heuristic",SYN,
                      "--search", "lazy(pareto([hff,hlm]))"],
        "eg0.05":    ["--heuristic",SYN,
                      "--search", "lazy(eg([hff,hlm],epsilon=0.05))"],
        "ucb0.1":    ["--heuristic",SYN,
                      "--search", "lazy(ucb([hff,hlm],k=0.1))"],
        "ed0.01":    ["--heuristic",SYN,
                      "--search", "lazy(ed([hff,hlm]))"],
        "softmax100":  ["--heuristic",SYN,
                        "--search", "lazy(softmax([hff,hlm]))"],
        "softmax10":   ["--heuristic",SYN,
                        "--search", "lazy(softmax([hff,hlm],t=10))"],
        "softmax1":    ["--heuristic",SYN,
                        "--search", "lazy(softmax([hff,hlm],t=1))"],
        "softmax0.1":  ["--heuristic",SYN,
                        "--search", "lazy(softmax([hff,hlm],t=0.1))"],
        # 
        # 
        # "eg0.2":     ["--search", "lazy(eg([ff(),cea()],epsilon=0.2))"],
        # "ucb2.0":    ["--search", "lazy(ucb([ff(),cea()],k=2.0))"],
        # "ed0.1":    ["--search", "lazy(ed([ff(),cea()],decay=0.1))"],
        # "ed0.001":    ["--search", "lazy(ed([ff(),cea()],decay=0.001))"],
        # # 
        # # "--ff":     ["--search", "lazy(single(ff()))"],
        # # "--cea":    ["--search", "lazy(single(cea()))"],
        # "alt":    ["--search", "lazy(alt([single(ff()),single(cea())]))"],
        # "pareto": ["--search", "lazy(pareto([ff(),cea()]))"],
        # "eg0.05":     ["--search", "lazy(eg([ff(),cea()],epsilon=0.05))"],
        # "ucb0.1":    ["--search", "lazy(ucb([ff(),cea()],k=0.1))"],
        # "ed0.01":    ["--search", "lazy(ed([ff(),cea()]))"],
        # "softmax100":    ["--search", "lazy(softmax([ff(),cea()]))"],
        # "softmax10":    ["--search", "lazy(softmax([ff(),cea()],t=10))"],
        # "softmax1":    ["--search", "lazy(softmax([ff(),cea()],t=1))"],
        # "softmax0.1":    ["--search", "lazy(softmax([ff(),cea()],t=0.1))"],
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
