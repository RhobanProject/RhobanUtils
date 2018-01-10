#pragma once

#include <functional>
#include <random>
#include <utility>
#include <vector>

namespace rhoban_utils
{

class MultiCore
{
public:
  /// In each pair, the first element is start, the second is end and the interval is [start,end[
  typedef std::vector<std::pair<int,int>> Intervals;

  typedef std::function<void(int start_idx, int end_idx)> Task;
  typedef std::function<void(int start_idx,
                             int end_idx,
                             std::default_random_engine * engine)> StochasticTask;
  

  static Intervals buildIntervals(int nb_tasks, int nb_threads);

  /// Can be used when a function 'f' needs to be run for all values in [0,nb_tasks[
  /// It should be safe to run the function from multiple thread at the same time
  static void runParallelTask(Task t,
                              int nb_tasks,
                              int nb_threads);

  /// Can be used when a function 'f' needs to be run for all values in [0,nb_tasks[
  /// It should be safe to run the function from multiple thread at the same time
  /// Since the task is stochastic, multiple engines have to be provided, the number
  /// of threads is defined by the size of the 'engines' vector
  static void runParallelStochasticTask(StochasticTask st,
                                        int nb_tasks,
                                        std::vector<std::default_random_engine> * engines);
};

}
