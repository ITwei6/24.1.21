#pragma once
#include <pthread.h>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
//线程池的本质就是生产消费模型
//一个生产者往线程池里放任务，然后其他消费者者就竞争这个任务执行
//线程池里有很多线程，所以它的基本属性肯定有识别线程的id


static const int defaultnum=3;//默认线程池里有3个线程
struct ThreadInfo
{
   pthread_t tid;
   std::string name;
};
template <class T>
class ThreadPool
{
public:

    void Lock()
    {
        pthread_mutex_lock(&_mutex);
    }
    void Unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }

    void Makeup()
    {
        pthread_cond_signal(&_cond);
    }
    bool isQueueEmpty()
    {
        return _task.empty();
    }
    void ThreadSleep()
    {
        pthread_cond_wait(&_cond,&_mutex);
    }
    T Pop()
    {
       T t=_task.front();
       _task.pop();
       return t;
    }
   
   std::string GetthreadName(pthread_t id)
   {
      for(const auto&ti :_thread)
      {
        if(ti.tid==id)
        return ti.name;
      }
      return "None";
   }
public:
     ThreadPool(int num=defaultnum):_thread(num)
     {
        pthread_mutex_init(&_mutex,nullptr);
        pthread_cond_init(&_cond,nullptr);
     }
     void Push(const T& in)//往线程池里发送任务,发送是没有条件的，但一旦发送了，就说明消费条件满足了，就要唤醒线程池里的线程去执行
     {
        Lock();
        _task.push(in);
        Makeup();//唤醒在条件变量下等待的线程
        Unlock();
     }

    
    
    //要注意，在类内部创建线程时，线程执行的函数里，会有this指针，不满足要求，所以必须要使用静态成员函数
    //这样才可以没有this指针，只有一个参数，但静态成员函数又不能访问类成员，所以在给线程函数传递参数时，我们传
    //该类的this指针，这样就可以通过this指针访问类成员
    static void *Handler(void* args)//去线程池里的任务队列里竞争任务
    {
      ThreadPool<T>* td=static_cast< ThreadPool<T>*>(args);
      std::string name=td->GetthreadName(pthread_self());//根据tid来获取到对应的名字
      //线程创建出来就去竞争任务，任务在哪里？在任务队列里，任务队列里没有怎么办？去条件变量下等待
      while(true)
      {
        td->Lock();

        while(td->isQueueEmpty())//防止伪唤醒
        {
            td->ThreadSleep();//没有任务那么就去条件变量下等待       
        }
       //如果有任务，那么就将任务拿出来，并执行
       T t=td->Pop();
       td->Unlock();
       
       t();//处理任务
      std::cout<<name<<"run, "<<"reslut: "<<t.Getresult()<<std::endl;
      }

    }
     //当我们去调用这个线程池的时候，线程池就应该给我们创建若干个线程在线程池里。而当有人往线程池里发送任务时，线程
     //池里的线程会立刻被唤醒，去竞争任务
     
     void Start()//线程池里的线程刚被创建出来，就会去线程池里的队列里竞争任务，如果没有任务，那么它就会去休眠
     {
      
      int num=_thread.size();
      for(int i=0;i<num;i++)
      {
         
        _thread[i].name="thread- "+std::to_string(i+1);

        pthread_create(&(_thread[i].tid),nullptr,Handler,this);
      }   
     }

     ~ThreadPool()
     {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
     }

private:
 

  std::vector<ThreadInfo> _thread;//根据这个来找到要分配任务的线程，线程池里存储的线程
  std::queue<T> _task;//线程池里存放的任务

  pthread_mutex_t _mutex;
  pthread_cond_t _cond;
   
};