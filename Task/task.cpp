#include <stdio.h>
#include "task.h"
#include "utils.h"

UInt32 TaskThreadPool::m_u32ThreadNum = 0;
UInt32 TaskThreadPool::m_u32ThreadPicker = 0;
ThreadVector TaskThreadPool::m_vecThreads;

void Task::Signal(Task::EventFlags i_Events, SInt64 i_Priority)
{
    i_Events |= kAlive;
    Task::EventFlags old_events = m_Events;
    m_Events |= i_Events;
    if (0 == (old_events & kAlive) && TaskThreadPool::GetThreadNum() > 0)
    {
        TaskThreadPool::AddTask(this, i_Priority);
    }
}

Task::EventFlags Task::GetEvents()
{
    Task::EventFlags events = m_Events & kAliveOff;
    m_Events -= events;
    return events;
}

void TaskThread::Entry()
{
    while (!m_bStop)
    {
        Task *pTask = WaitForTask();
        if (NULL == pTask)
        {
            break;
        }

        SInt64 ret = pTask->Run();
        pTask->ResetEvents();
        if (ret > 0)
        {
            pTask->Signal(Task::kIdleEvent, ret);
        }
        else if (ret < 0)
        {
            delete pTask;
            pTask = NULL;
        }
        ThreadYield();
    }
}

void TaskThread::AddTask(Task* i_pTask, SInt64 i_Priority)
{
    i_Priority = i_Priority*1000 + MicroSeconds();
    T_Node n(i_Priority, i_pTask);
    m_TaskQueue.push(n);
}

Task* TaskThread::WaitForTask()
{
    while (!m_bStop)
    {
        if (m_TaskQueue.empty())
        {
            SleepUs(10000); // sleep 10ms
        }
        else
        {
            const T_Node &n = m_TaskQueue.top();
            Task* pTask = (Task*)n.pValue;
            UInt64 t = MicroSeconds();
            if (n.u64Priority > t)
            {
                SleepUs(n.u64Priority - t);
            }
            m_TaskQueue.pop();
            return pTask;
        }
    }

    return NULL;
}

void TaskThreadPool::AddThreads(UInt32 i_u32Num)
{
    if (m_vecThreads.empty())
    {
        m_u32ThreadNum = i_u32Num;
        for (SInt32 i = 0; i < i_u32Num; i ++)
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
        m_vecThreads.erase(it);
    }
}

void TaskThreadPool::AddTask(Task* i_pTask, SInt64 i_Priority)
{
    UInt32 u32Idx = (++m_u32ThreadPicker) % m_u32ThreadNum;
    TaskThread *pThread = m_vecThreads[u32Idx];
    if (pThread)
    {
        pThread->AddTask(i_pTask, i_Priority);
    }
}


