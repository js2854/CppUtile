#include "Log.hh"

#include <time.h>           //strftime
#include <assert.h>
#include <stdarg.h>         //vsnprintf,va_start,va_end

#ifdef WIN32
#include <io.h>             //access
#include <direct.h>         //_mkdir
#define MKDIR(a)            _mkdir((a))
#define PATH_SEPARATER      '\\'
#else
#include <unistd.h>         //access
#include <sys/stat.h>       //mkdir
#include <sys/time.h>       //gettimeofday
#define MKDIR(a)            mkdir((a),0755)
#define PATH_SEPARATER      '/'

#include <sys/syscall.h>    //for pid_t, syscall
pid_t gettid() {  return syscall(SYS_gettid); }
#endif

#define DEFAULT_LOG_SIZE    (10*1024*1024)//10M
#define MAX_PATH            512

#define NEW_LINE            "\r\n"

static char* s_level_str[] = {"ERROR", "WARNING", "INFO", "", "TRACE"};

#define FCLOSE(fp)    {if (NULL != fp) {fclose(fp); fp = NULL;}}

CLog::CLog(void)
{
    m_log_filename = "app.log";
    m_fp = NULL;
    m_log_level = LOG_INFO;
    m_log_size = DEFAULT_LOG_SIZE;
}

CLog::~CLog(void)
{
    FCLOSE(m_fp);
}

void CLog::init()
{
    assert(m_log_filename.length() > 0);

    FCLOSE(m_fp);    
    rename_file();

    m_fp = fopen(m_log_filename.c_str(), "a+b");
    if (NULL == m_fp)
    {
        fprintf(stderr, "Init log file(%s) fail!", m_log_filename.c_str());
    }
}

bool CLog::mk_dir()
{
    assert(m_log_filename.length() > 0);

    char* p_str = (char*)m_log_filename.c_str();
    int len = m_log_filename.find_last_of(PATH_SEPARATER);
    if (string::npos == len) return false;//找不到路径分隔符直接返回

    char dir[MAX_PATH] = {0};
    strncpy(dir, p_str, ++len);

    if (0 != access(dir, 0))
    {
        //创建目录(TODO: "./","../"等相对路径的情况未处理) 
        for (int i = 1; i < len; i++)  
        {
            if (dir[i] == PATH_SEPARATER)
            {
                dir[i] = '\0';
                //puts(dir);

                if ((0 != access(dir, 0)) && (0 != MKDIR(dir)))
                {//如果目录不存在,且创建失败
                    return false;
                }  

                dir[i] = PATH_SEPARATER;
            }
        }
    }    

    return true;  
}

char* CLog::get_time_str(bool is_write)
{
    time_t now = {0};
    struct tm *ptime = NULL;
    time(&now); 
    ptime = localtime(&now);
    memset(m_time_str, 0, sizeof(m_time_str));

    uint32 milisec = 0;
#ifdef WIN32
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    milisec = wtm.wMilliseconds;
#else
    struct timeval tv = {0};
    gettimeofday(&tv, 0);
    milisec = tv.tv_usec/1000;
#endif

    if (is_write)
    {//用来写日志
        sprintf(m_time_str, "%04d%02d%02d-%02d:%02d:%02d.%06ld",
            (1900+ptime->tm_year), (1+ptime->tm_mon), ptime->tm_mday,
            ptime->tm_hour, ptime->tm_min, ptime->tm_sec, milisec);
    } 
    else
    {//用来重命名文件
        sprintf(m_time_str, "%04d-%02d-%02d-%02d_%02d_%02d_%06ld",
            (1900+ptime->tm_year), (1+ptime->tm_mon), ptime->tm_mday,
            ptime->tm_hour, ptime->tm_min, ptime->tm_sec, milisec);
    }
    
    return m_time_str;
}

bool CLog::rename_file()
{
    string new_name = m_log_filename + "-" + get_time_str(false);
    return (0 == rename(m_log_filename.c_str(), new_name.c_str()));
}

int CLog::get_thread_id()
{
#ifdef WIN32
    return GetCurrentThreadId();
#else
    return gettid();
#endif
}

int CLog::writeline(uint level, const char* format_str, ...)
{    
    if (m_log_level < level) return 0;
    
    m_lock.lock();
    assert(NULL != m_fp);
    if (ftell(m_fp) >= m_log_size) init();//切换日志写入

    int write_len = fprintf(m_fp, "[%s-%d][%s]", get_time_str(true), get_thread_id(), s_level_str[level/2]);

    va_list p_list;
    va_start(p_list, format_str);
    write_len += vfprintf(m_fp, format_str, p_list);
    va_end(p_list);
    write_len += fprintf(m_fp, "\n");
    fflush(m_fp);
    m_lock.unlock();

    return write_len;
}

log_inst    CLogFactory::m_inst_list[MAX_LOG_INSTANCE] = {0};
uint        CLogFactory::m_inst_num = 0;
MutexLock   CLogFactory::m_lock;

CLogFactory::CLogFactory(void)
{
}

CLogFactory::~CLogFactory(void)
{
    free_all_inst();
}

CLog* CLogFactory::get_instance(const char* name)
{
    CLog* plog = NULL;    
    for (int i = 0; i < m_inst_num; i++)
    {
        if (0 == strcmp(m_inst_list[i].name, name))
        {
            plog = m_inst_list[i].plog;
            break;
        }
    }

    if ((NULL == plog) && (m_inst_num < MAX_LOG_INSTANCE))
    {
        m_lock.lock();
        plog = new CLog();
        strcpy(m_inst_list[m_inst_num].name, name);
        m_inst_list[m_inst_num].plog = plog;
        m_inst_num++;
        m_lock.unlock();
    }
    
    return plog;
}

void CLogFactory::free_instance(const char* name)
{
    m_lock.lock();
    for (int i = 0; i < m_inst_num; i++)
    {
        if (m_inst_list[i].name == name)
        {
            delete m_inst_list[i].plog;
            memset(m_inst_list[i].name, 0, sizeof(m_inst_list[i].name));
            m_inst_list[i].plog = NULL;
            break;
        }
    }
    m_lock.unlock();
}

void CLogFactory::free_all_inst()
{
    m_lock.lock();
    for (int i = 0; i < m_inst_num; i++)
    {
        delete m_inst_list[i].plog;
        memset(m_inst_list[i].name, 0, sizeof(m_inst_list[i].name));
        m_inst_list[i].plog = NULL;        
    }
    m_lock.unlock();
}

