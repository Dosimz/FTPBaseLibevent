// SyncResources.cpp
#include "SyncResources.h"

std::mutex mtx; // 定义互斥锁
std::condition_variable cv; // 定义条件变量
bool ready = false; // 初始化控制变量

