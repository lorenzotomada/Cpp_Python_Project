import sys
sys.path.append('../')
sys.path.append('../../../Python_Code/')
sys.path.append('../../../build/')


from integration_test import main as main_test

import os
import cProfile   # https://docs.python.org/3/library/profile.html
import pstats


print('We will profile the code using cProfile. This will also execute integration_test.py')


if __name__ == "__main__":

    profiling_results_path = "integration_profile_results.prof"  # Save the results in the current working directory

    profiler = cProfile.Profile()
    profiler.enable()

    main_test()

    profiler.disable()

    profiler.dump_stats(profiling_results_path)

    stats = pstats.Stats(profiling_results_path)  # Print the results
    stats.sort_stats("cumulative")
    stats.print_stats()

    print("Profiling results saved to", profiling_results_path)