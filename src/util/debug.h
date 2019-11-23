#pragma once

#ifndef NDEBUG
#define TRY(id, task, on_err)                                \
  try {                                                      \
    task;                                                    \
  } catch (const std::exception &e) {                        \
    std::cerr << id << " FAILED: " << e.what() << std::endl; \
    on_err;                                                  \
  }
#define DEBUG 1
#else
#define TRY(id, task, on_err) task;
#endif