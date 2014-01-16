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
    //设置日志级别
    SET_LOG_LEVEL(LOG_LEVEL_TRACE);

    //设置日志大小
    SET_LOG_SIZE(10 * 1024 * 1024);

    //设置占用磁盘空间大小
    SET_LOG_SPACE(100 * 1024 * 1024);

    clock_t start, finish;
    double duration;

    start = clock();

    for(int i = 0; i < 1000000; i++)
    {
        LOG_TRACE("****%d****", i);
        LOG_INFO("test INFO");
        LOG_WARNING("test WARNING");
        LOG_ERROR("test ERROR");
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("duration = %.3f.\n", duration);

    getchar();

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
        LOG_TRACE(pstr, "%s ****%d****", pstr, i);
        LOG_INFO(pstr, "%s test INFO", pstr);
        LOG_WARNING(pstr, "%s test WARNING", pstr);
        LOG_ERROR(pstr, "%s test ERROR", pstr);
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("duration = %.3f.\n", duration);

    fflush(stdout);
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
    //设置日志级别
    SET_LOG_LEVEL(TESTMODULE1, LOG_LEVEL_TRACE);
    SET_LOG_LEVEL(TESTMODULE2, LOG_LEVEL_TRACE);

    //设置日志大小
    SET_LOG_SIZE(TESTMODULE1, 1 * 1024 * 1024);
    SET_LOG_SIZE(TESTMODULE2, 1 * 1024 * 1024);

    //设置占用磁盘空间大小
    SET_LOG_SPACE(TESTMODULE1, 10 * 1024 * 1024);
    SET_LOG_SPACE(TESTMODULE2, 10 * 1024 * 1024);

#ifdef WIN32
    _beginthread(threadfunc, NULL, TESTMODULE1);
    _beginthread(threadfunc, NULL, TESTMODULE2);
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
