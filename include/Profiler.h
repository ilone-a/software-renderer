#pragma once
#include <chrono>
#include <optional>
#include <algorithm>
#include <string>
//#include "ObjectLoader.h"
//#include "MathUtils.h"
using namespace std::chrono;
using namespace std;


	struct Profiler {
		std::string name;
		float fps;
		high_resolution_clock::time_point start;

		Profiler(const std::string& funcName) : name(funcName), start(high_resolution_clock::now()) {}

		~Profiler() {
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
			fps = 1 / duration;
			if (duration > 2e-6) { // 2 микросекунды
				//printf("%s took %.6f seconds\n", name.c_str(), duration);
				printf("%s took %.3f frames per second\n", name.c_str(), fps);
			};
		}
	};