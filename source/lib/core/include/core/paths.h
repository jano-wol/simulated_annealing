#ifndef SIMULATED_ANNEALING_CORE_PATHS_H_
#define SIMULATED_ANNEALING_CORE_PATHS_H_

#include <string>

namespace sa::core
{
	std::string getRepoFolder();
	std::string getBuildFolder();
	std::string getBuildDataFolder();
	std::string getBuildTestDataFolder();
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_PATHS_H_
