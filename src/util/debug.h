#pragma once

#ifndef NDEBUG

#include <iostream>

#define DEBUG 1

#define TRY(id, task, on_err)                                \
	try {                                                      \
		task;                                                    \
	} catch (const std::exception &e) {                        \
		std::cerr << id << " failed: " << e.what() << std::endl; \
		on_err;                                                  \
	}
	
#define LOG(message) std::cout << "LOG: " << message << std::endl;

#else
#define TRY(id, task, on_err) task;
#define LOG(message);
#endif