cmake_minimum_required(VERSION 3.12)
project(thread_pool)

set(CMAKE_CXX_STANDARD 17)

add_library(thread_pool thread_safe_queue.h thread_pool.cc thread_pool.h)
