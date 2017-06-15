cmake_minimum_required(VERSION 3.6)
project(threadLibrary)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c tqueue.c tqueue.h bthread.h bthread_private.h bthread.c bthread_private.c bthread_test.c bthread_test.h bthread.c tmux.c tmutex.h tsemapthore.h tbarrier.h tcondition.h tbarrier.c tcondition.c tsemaphore.c)
add_executable(threadLibrary ${SOURCE_FILES})