#ifndef __TASK_H__
#define __TASK_H__

#include <vector>
#include "types.h"
#include "thread.h"
using std::vector;

class Task
{
public:
    Task() : m_Events(0) {}
    virtual ~Task() {}

    typedef unsigned int EventFlags;
    // EVENTS
    // here are all the events that can be sent to a task
    enum
    {
        kKillEvent    = 0x1 << 0x0,
        kIdleEvent    = 0x1 << 0x1,
        kStartEvent   = 0x1 << 0x2,
        kTimeoutEvent = 0x1 << 0x3,

        // socket events
        kReadEvent    = 0x1 << 0x4,
        kWriteEvent   = 0x1 << 0x5,

        // update event
        kUpdateEvent  = 0x1 << 0x6,
        kMsgEvent     = 0x1 << 0x7
    };

    // return:
    // >0 invoke me after this number of MilSecs with a kIdleEvent
    // 0 don't reinvoke me at all.
    // -1 delete me
    virtual SInt64 Run() = 0;

    // Send an event to this task.
    void Signal(Task::EventFlags i_Events, SInt64 i_s64Priority=0);

    EventFlags GetEvents();

    void ResetEvents() { m_Events = 0; }
private:
    enum
    {
        kAlive    = 0x80000000, //EventFlags, again
        kAliveOff = 0x7fffffff
    };
    Task::EventFlags m_Events;
};

class TaskThread: public Thread
{
public:
    TaskThread() : Thread(), m_bStop(false)
    {
    }
    ~TaskThread()
    {
        m_bStop = true;
    }

    void Entry();

    void AddTask(Task* i_pTask, SInt64 i_s64Priority=0);

private:
    Task* WaitForTask();
private:
    PriorityQueue m_TaskQueue;
    bool m_bStop;
};

typedef vector<TaskThread*>  ThreadVector;

class TaskThreadPool
{
public:
    static void AddThreads(UInt32 i_u32Num);
    static void RemoveThreads();
    static void AddTask(Task* i_pTask, SInt64 i_s64Priority=0);
    static SInt32 GetThreadNum() { return m_u32ThreadNum; }
private:
    static UInt32 m_u32ThreadNum;
    static UInt32 m_u32ThreadPicker;
    static ThreadVector m_vecThreads;
};

#endif
