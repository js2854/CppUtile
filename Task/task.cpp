#include <stdio.h>
#include "task.h"
#include "utils.h"

#define MIN_WAIT_TIME_MS        10
#define MAX_RUN_TIMES_ONCE      10

UInt32 TaskThreadPool::m_u32ThreadNum = 0;
UInt32 TaskThreadPool::m_u32ThreadPicker = 0;
ThreadVector TaskThreadPool::m_vecThreads;

void Task::PushEvents(Task::EventFlags i_Events, SInt64 i_Priority)
{
    m_Events |= i_Events;
    TRACE("0.PushEvents: Task %p, i_Events %d, i_Priority %lld, IsAlive %d, ThreadNum %d",
          this, i_Events, i_Priority, IsAlive() ? 1 : 0, TaskThreadPool::GetThreadNum());
    if (IsAlive() && TaskThreadPool::GetThreadNum() > 0)
    {
        TRACE("1.PushEvents: Task %p, i_Events %d, i_Priority %lld", this, i_Events, i_Priority);
        TaskThreadPool::AddTask(this, i_Priority);
        TRACE("2.PushEvents: Task %p, i_Events %d, i_Priority %lld", this, i_Events, i_Priority);
    }
}

Task::EventFlags Task::PopEvents()
{
    Task::EventFlags events = m_Events;
    m_Events -= events;
    return events;
}

TaskThread::~TaskThread()
{
    Stop();
    AutoMutex mutex(&m_lock);
    while (!m_TaskQueue.empty())
    {
        m_TaskQueue.pop();
    }
    m_TaskSet.clear();
    while (!m_TimeoutTaskQueue.empty())
    {
        m_TimeoutTaskQueue.pop();
    }
}

void TaskThread::Entry()
{
    while (!IsStop())
    {
        Task *pTask = WaitForTask();
        if (NULL == pTask || IsStop())
        {
            break;
        }
        TRACE("GetTask %p", pTask);
        SInt32 s32MaxRunTimes = MAX_RUN_TIMES_ONCE;
        while (s32MaxRunTimes-- > 0 && pTask->IsAlive() && pTask->GetEvents() > 0)
        {
            SInt64 ret = pTask->Run();
            if (ret > 0)
            {
                s32MaxRunTimes = 0;
                pTask->PushEvents(Task::kIdleEvent, ret);
            }
            else if (ret < 0)
            {
                s32MaxRunTimes = 0;
                delete pTask;
                pTask = NULL;
            }
            ThreadYield();
        }
    }
}

void TaskThread::AddTask(Task* i_pTask, SInt64 i_Priority)
{
    if (i_Priority > 0)
    {
        i_Priority = i_Priority*US_IN_1MS + MicroSeconds();
        T_Node n(i_Priority, i_pTask);
        AutoMutex mutex(&m_lock);
        m_TimeoutTaskQueue.push(n);
    }
    else if (m_TaskSet.find(i_pTask) == m_TaskSet.end())
    {
        {
            AutoMutex mutex(&m_lock);
            m_TaskQueue.push(i_pTask);
            m_TaskSet.insert(i_pTask);
        }
        m_cond.Signal();
    }
    TRACE("AddTask: pTask %p, Time %lld", i_pTask, i_Priority);
}

Task* TaskThread::WaitForTask()
{
    while (!IsStop())
    {
        UInt64 u64CurTime = MicroSeconds();
        if (!m_TimeoutTaskQueue.empty() && m_TimeoutTaskQueue.top().u64Priority <= u64CurTime)
        {
            AutoMutex mutex(&m_lock);
            const T_Node &n = m_TimeoutTaskQueue.top();
            Task* pTask = (Task*)n.pValue;
            m_TimeoutTaskQueue.pop();
            return pTask;
        }
        else
        {
            AutoMutex mutex(&m_lock);
            if (m_TaskQueue.empty())
            {
                UInt64 u64Timeout = 0;
                if (!m_TimeoutTaskQueue.empty())
                {
                    u64Timeout = (m_TimeoutTaskQueue.top().u64Priority - u64CurTime)/US_IN_1MS;
                }
                u64Timeout = std::max((UInt64)MIN_WAIT_TIME_MS, u64Timeout);
                // TRACE("wait %llu ms", u64Timeout);
                m_cond.Wait(m_lock, u64Timeout);
                // TRACE("wait timeout");
            }
            if (!m_TaskQueue.empty())
            {
                Task* pTask = m_TaskQueue.front();
                m_TaskQueue.pop();
                m_TaskSet.erase(pTask);
                return pTask;
            }
        }
        ThreadYield();
    }

    return NULL;
}

void TaskThreadPool::AddThreads(UInt32 i_u32Num)
{
    if (m_vecThreads.empty())
    {
        m_u32ThreadNum = i_u32Num;
        for (UInt32 i = 0; i < i_u32Num; i ++)
        {
            TaskThread *pThread = new TaskThread();
            m_vecThreads.push_back(pThread);
            pThread->Start();
        }
    }
}

void TaskThreadPool::RemoveThreads()
{
    ThreadVector::iterator it = m_vecThreads.begin();
    for (; it != m_vecThreads.end(); it++)
    {
        TaskThread *pThread = *it;
        delete pThread;
        pThread = NULL;
    }
    m_vecThreads.clear();
}

void TaskThreadPool::AddTask(Task* i_pTask, SInt64 i_Priority)
{
    UInt32 u32Idx = (++m_u32ThreadPicker) % m_u32ThreadNum;
    TaskThread *pThread = m_vecThreads[u32Idx];
    if (pThread && !pThread->IsStop())
    {
        pThread->AddTask(i_pTask, i_Priority);
    }
}


