#! /usr/bin/env python

from __future__ import print_function

import multiprocessing
import os
import subprocess
import sys

import configs

DIR = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(DIR))
BENCHMARKS_DIR = os.path.join(REPO, "benchmarks")
FAST_DOWNWARD = os.path.join(REPO, "fast-downward.py")

TASKS = [os.path.join(BENCHMARKS_DIR, path) for path in [
    "visitall-sat11-strips/problem12.pddl",
    "transport-sat11-strips/p01.pddl",
    "woodworking-sat11-strips/p03.pddl",
    "miconic/s1-0.pddl",
    "miconic/s10-0.pddl",
    "airport-adl/p01-airport1-p1.pddl",
    "elevators-opt08-strips/p01.pddl",
    # "elevators-opt11-strips/p01.pddl",
    "openstacks-opt08-strips/p01.pddl",
    "driverlog/pfile1",
    "parking-opt11-strips/pfile03-011.pddl",
    "driverlog/pfile1",
]]

CONFIGS = {}
# CONFIGS.update(configs.default_configs_optimal(core=False, ipc=False, extended=False))
CONFIGS.update(configs.default_configs_satisficing(core=False, ipc=False, extended=True))
# CONFIGS.update(configs.task_transformation_test_configs())
# CONFIGS.update(configs.regression_test_configs())

if os.name == "nt":
    # No support for portfolios on Windows
    del CONFIGS["seq_opt_merge_and_shrink"]
    del CONFIGS["seq_opt_fdss_1"]
    del CONFIGS["seq_opt_fdss_2"]
    del CONFIGS["seq_sat_lama_2011"]
    del CONFIGS["seq_sat_fdss_1"]
    del CONFIGS["seq_sat_fdss_2"]

def run_plan_script(task, nick, config, debug):
    cmd = [sys.executable, FAST_DOWNWARD]
    if os.name != "nt":
        cmd.extend(["--search-time-limit", "30m"])
    if debug:
        cmd.append("--debug")
    if "--alias" in config:
        assert len(config) == 2, config
        cmd += config + [task]
    else:
        cmd += [task] + config
    print("\nRun: {}".format(nick))
    sys.stdout.flush()
    subprocess.check_call(cmd)


def cleanup():
    subprocess.check_call([sys.executable, FAST_DOWNWARD, "--cleanup"])


def main():
    # On Windows, ./build.py has to be called from the correct environment.
    # Since we want this script to work even when we are in a regular
    # shell, we do not build on Windows. If the planner is not yet built,
    # the driver script will complain about this.
    for task in TASKS:
        print("\nTask: {}".format(task))
        for nick, config in CONFIGS.items():
            for debug in [False]:
                try:
                    run_plan_script(task, nick, config, debug)
                except subprocess.CalledProcessError:
                    sys.exit(
                        "\nError: {} failed to solve {} (debug={})".format(
                            nick, task, debug))
                cleanup()

main()
