#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <zna/source/thirdParty/gtoolbox/test/test.h>

#if env_os_ios or env_os_osx
    #include <unistd.h>
    #include <sys/sysctl.h>
#endif

namespace test {

    void
    abort()
    {
        std::exit(1);
    }

    void
    flush()
    {
        std::fflush(stdout);
    }

    int
    getpid()
    {
        return ::getpid();
    }

    bool
    debugger_attached()
    {
        #if !Z2_ANDROID_HACK_FIXME
        int               mib[4];
        struct kinfo_proc info;
        size_t            size;
        
        // Initialize the flags so that, if sysctl fails for some bizarre 
        // reason, we get a predictable result.
        
        info.kp_proc.p_flag = 0;
        
        // Initialize mib, which tells sysctl the info we want, in this case
        // we're looking for information about a specific process ID.
        
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PID;
        mib[3] = getpid();
        
        // Call sysctl.
        
        size = sizeof(info);
        sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
        
        // We're being debugged if the P_TRACED flag is set.
        
        return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
        #else
        return false;
        #endif // !Z2_ANDROID_HACK_FIXME
    }

    static
    void
    log_failure(
        const char*    const func,
        const char*    const file,
        const unsigned       line,
        const char*    const expr,
        const char*    const test)
    {
        const char* func_name = func + strlen(func);

        while (
            func_name     != func and
            func_name[-1] != ' ')
            func_name -= 1;

        const char* file_name = file + strlen(file);

        while (
            file_name     != file and
            file_name[-1] != '/'  and
            file_name[-1] != '\\')
            file_name -= 1;

        printf(
            "%s failed: %s(%s) @ %s:%u\n",
            func_name, test, expr,
            file_name, line);
    }

    void
    assert_failed(
        const char*    const func,
        const char*    const file,
        const unsigned       line,
        const char*    const expr)
    {
        log_failure(func, file, line, expr, "test_assert");
    }

    void
    check_failed(
        const char*    const func,
        const char*    const file,
        const unsigned       line,
        const char*    const expr)
    {
        log_failure(func, file, line, expr, "test_check");
    }

} // namespace test
