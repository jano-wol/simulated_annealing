#ifndef SIMULATED_ANNEALING_SA_STUDIO_THREAD_POOL_MANAGER_H_
#define SIMULATED_ANNEALING_SA_STUDIO_THREAD_POOL_MANAGER_H_

#include <bs/BS_thread_pool.hpp>

class ThreadPoolManager
{
public:
  static BS::thread_pool<0>& getPool()
  {
    static BS::thread_pool<0> pool;
    return pool;
  }
};

#endif  // SIMULATED_ANNEALING_SA_STUDIO_THREAD_POOL_MANAGER_H_
