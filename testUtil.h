#pragma once

#include <iostream>
using namespace std;
#include <shared_mutex>

// 声明一个全局的读写锁
extern std::shared_mutex rw_mutex;
//// 定义一个全局的读写锁
//std::shared_mutex rw_mutex;
#ifdef TEST
#define testout(msg) cout << msg << endl << flush
#else 
#define testout(msg) 
#endif
