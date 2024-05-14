// SyncResources.h
#ifndef SYNC_RESOURCES_H
#define SYNC_RESOURCES_H

#include <mutex>
#include <condition_variable>

extern std::mutex mtx; // 声明互斥锁
extern std::condition_variable cv; // 声明条件变量
extern bool ready; // 控制变量，用于通知条件

#endif // SYNC_RESOURCES_H

