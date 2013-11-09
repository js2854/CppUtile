#include <time.h> //clock_t,clock(),CLOCKS_PER_SEC

#ifdef WIN32
#include <process.h>//_beginthread
#else
#include <pthread.h>//pthread_create
#endif


#include "Log.hh"

#ifdef SINGLE_LOG
int main()
{
#ifdef linux
    SET_LOG_NAME("/home/jaxon/some_test/mylog/temp/test.log");    
#else    
    SET_LOG_NAME("C:\\test\\test.log");
#endif
    //日志级别设置
    SET_LOG_LEVEL(LOG_TRACE);

    //日志大小设置
    SET_LOG_SIZE(1 * 1024 * 1024);

    clock_t start, finish;
    double duration;
    
    start = clock();

    for(int i = 0; i < 100000; i++)
    {
        TRACE("****%d****", i);
        INFO("test INFO");
        WARNING("test WARNING");
        ERROR("test ERROR");
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC; 
    printf("duration = %.3f.\n", duration);

    return 0;
}

#else

//需要用户在此定义模块名有宏
//e.g.
#define TESTMODULE1    "TestModule1"
#define TESTMODULE2    "TestModule2"

#ifdef WIN32
void threadfunc(void *param)
#else
void* threadfunc(void *param)
#endif
{
    clock_t start, finish;
    double duration;
    char* pstr = (char*)param;

    start = clock();

    for(int i = 0; i < 100000; i++)
    {
        TRACE(pstr, "%s ****%d****", pstr, i);
        INFO(pstr, "%s test INFO", pstr);
        WARNING(pstr, "%s test WARNING", pstr);
        ERROR(pstr, "%s test ERROR", pstr);
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC; 
    printf("duration = %.3f.\n", duration);
}

int main()
{
#ifdef linux
    SET_LOG_NAME(TESTMODULE1, "/home/jaxon/some_test/mylog/temp/test1.log");
    SET_LOG_NAME(TESTMODULE2, "/home/jaxon/some_test/mylog/temp/test2.log");
#else    
    SET_LOG_NAME(TESTMODULE1, "C:\\test\\test1.log");
    SET_LOG_NAME(TESTMODULE2, "C:\\test\\test2.log");
#endif
    //日志级别设置
    SET_LOG_LEVEL(TESTMODULE1, LOG_TRACE);
    SET_LOG_LEVEL(TESTMODULE2, LOG_TRACE);

    //日志大小设置
    SET_LOG_SIZE(TESTMODULE1, 1 * 1024 * 1024);
    SET_LOG_SIZE(TESTMODULE2, 1 * 1024 * 1024);

#ifdef WIN32
    _beginthread(threadfunc, NULL, TESTMODULE1);
    //_beginthread(threadfunc, NULL, TESTMODULE1);
#else
    pthread_t handle1 = 0;
    pthread_t handle2 = 0;
    int ret = pthread_create(&handle1, (pthread_attr_t*)0, threadfunc, (void*)TESTMODULE1);
    printf("create thread 1. ret = %d, handle=%d\n", ret, handle1);
    ret = pthread_create(&handle2, (pthread_attr_t*)0, threadfunc, (void*)TESTMODULE2);
    printf("create thread 2. ret = %d, handle=%d\n", ret, handle2);
#endif

    getchar();

    return 0;
}
#endif
