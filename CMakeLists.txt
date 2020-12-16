cmake_minimum_required(VERSION 3.0.0)
project(ex1 VERSION 0.1.0 LANGUAGES C)
set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")
set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})
link_directories(.)
add_executable(eventmanager date.h date.c event.h event.c member.h member.c event_manager.h event_manager.c priority_queue.h main.c)
target_link_libraries(eventmanager libpriority_queue.a)


