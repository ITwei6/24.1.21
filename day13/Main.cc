#include <iostream>
#include "ThreadPool.hpp"
#include "TASK.hpp"
#include <ctime>
#include <unistd.h>
int main()
{

    ThreadPool<TASK> *tp = new ThreadPool<TASK>();
    tp->Start();
    int len = opera.size();
    srand(time(nullptr));
    while (true)
    {
        // 1.获取数据
        int x = rand() % 10 + 1;
        usleep(10);
        int y = rand() % 10;
        char op = opera[rand() % len];
        TASK t(x, y, op);
        // 2.生产数据
        tp->Push(t);

        std::cout<<"main thread make task"<<t.GetTASK()<<std::endl;

        sleep(1);
    }
}