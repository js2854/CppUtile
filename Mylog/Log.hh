/*********************************************************************
*
* Copyright (c) 2013, UTStarcom (ShenZhen) Technology Ltd.
*       All rights reserved.
*
* Filename: Log.hh
* Abstract: Describe the purpose of the file
*
* Version : 0.1
* Author  : Jaxon Jia (UTSC0365)
* Date    : 2013/09/03
*
**********************************************************************/
#ifndef __Log_h__
#define __Log_h__

#include <string>
#include <stdio.h>
using namespace std;

////单实例API
#define SET_SINGLE_LOG_NAME(filepath)			SET_LOG_NAME("DEFAULT", filepath)
#define SET_SINGLE_LOG_LEVEL(level)				SET_LOG_LEVEL("DEFAULT", level)
#define SET_SINGLE_LOG_SIZE(size)				SET_LOG_SIZE("DEFAULT", size) 

//多实例API
#define SET_LOG_NAME(module, filepath)		CLogFactory::get_instance(module)->set_log_filepath(filepath)
#define SET_LOG_LEVEL(module, level)		CLogFactory::get_instance(module)->set_log_level(level)
#define SET_LOG_SIZE(module, size)			CLogFactory::get_instance(module)->set_log_size(size)

#define TRACE(module, format, ...)			CLogFactory::get_instance(module)->writeline(LOG_LEVEL_TRACE, format, ## __VA_ARGS__)
#define INFO(module, format, ...)			CLogFactory::get_instance(module)->writeline(LOG_LEVEL_INFO, format, ## __VA_ARGS__)
#define WARNING(module, format, ...)		CLogFactory::get_instance(module)->writeline(LOG_LEVEL_WARNING, format, ## __VA_ARGS__)
#define ERROR(module, format, ...)			CLogFactory::get_instance(module)->writeline(LOG_LEVEL_ERROR, format, ## __VA_ARGS__)

//日志级别
enum _log_level
{
	LOG_LEVEL_ERROR		= 1,  //错误
	LOG_LEVEL_WARNING	= 2,  //警告
	LOG_LEVEL_INFO      = 4,  //普通
	LOG_LEVEL_TRACE     = 8,  //调试
	LOG_LEVEL_MAX
};

//用户日志级别设置
#define LOG_ERROR        LOG_LEVEL_ERROR
#define LOG_WARNING      (LOG_ERROR | LOG_LEVEL_WARNING)
#define LOG_INFO         (LOG_WARNING | LOG_LEVEL_INFO)
#define LOG_TRACE        (LOG_INFO | LOG_LEVEL_TRACE)

#ifndef __MY_LOCK__
#define __MY_LOCK__

#ifdef WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif

class MutexLock
{
#ifdef WIN32
private:
	CRITICAL_SECTION m_lock;
public:
	MutexLock(void) {InitializeCriticalSection(&m_lock);}
	virtual ~MutexLock(void) {DeleteCriticalSection(&m_lock);}
	void lock() {EnterCriticalSection(&m_lock);}
	void unlock() {LeaveCriticalSection(&m_lock);}
#else
private:
	pthread_mutex_t m_lock;
public:
	MutexLock(void) {pthread_mutex_init(&m_lock, NULL);}
	virtual ~MutexLock(void) {pthread_mutex_destroy(&m_lock);}
	void lock() {pthread_mutex_lock(&m_lock);}
	void unlock() {pthread_mutex_unlock(&m_lock);}
#endif
};
#endif //__MY_LOCK__

typedef unsigned int  uint;
typedef unsigned long uint32;

#define MAX_TIME_STR_LEN	30

class CLog
{
public:
	CLog(void);
	~CLog(void);

	//设置日志文件路径
	void set_log_filepath(const string filepath) {m_log_filename = filepath; mk_dir(); init();}

	//设置日志文件大小切换
	void set_log_size(uint size) {m_log_size = size;}

	//设置日志级别
	void set_log_level(uint level) {m_log_level = (level > LOG_LEVEL_MAX) ? LOG_LEVEL_MAX : level;}
	
	//写入一行日志
	int writeline(uint level, const char* format_str, ...);

private:
	void	init();									//初始化操作
	bool	mk_dir();								//检查目录是否存在,不存在的话循环创建
	char*	get_time_str(bool is_write=true);		//获取当前时间字符串
	bool	rename_file();							//取当前时间重命名日志文件

private:
	MutexLock	m_lock;								//同步锁,用于多线程同步写
	string		m_log_filename;						//日志文件路径名
	FILE*		m_fp;								//日志文件句柄
	uint		m_log_level;						//设置的日志级别
	uint		m_log_size;							//设置的日志文件大小
	
	char		m_time_str[MAX_TIME_STR_LEN];		//写缓冲
};

#define MAX_LOG_INSTANCE	10
typedef struct _log_instance
{
	char* name;
	CLog* plog;
}log_inst;

class CLogFactory
{
public:
	CLogFactory(void);
	virtual ~CLogFactory(void);

	static CLog* get_instance(const char* name);
	static void  free_instance(const char* name);

private:
	static void  free_all_inst();

private:
	static log_inst		m_inst_list[MAX_LOG_INSTANCE];	//log实例列表
	static uint			m_inst_num;						//log实例数量
	static MutexLock	m_lock;							//同步锁,用于多线程同步写
};

#endif // __Log_h__

