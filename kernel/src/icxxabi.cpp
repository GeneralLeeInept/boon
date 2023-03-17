#include "monitor.h"

#define ATEXIT_MAX_FUNCS    128

typedef void (*atexit_func)(void*);

struct atexit_func_entry_t
{
    atexit_func destructor_func;
    void *obj_ptr;
    void *dso_handle;
};

static atexit_func_entry_t s_atexit_funcs[ATEXIT_MAX_FUNCS];
static unsigned int s_atexit_funcs_count = 0;

extern "C" int __cxa_atexit(atexit_func destructor_func, void* obj_ptr, void* dso_handle)
{
    if (s_atexit_funcs_count >= ATEXIT_MAX_FUNCS)
    {
        return -1;
    }

    unsigned int idx = s_atexit_funcs_count++;
    s_atexit_funcs[idx].destructor_func = destructor_func;
    s_atexit_funcs[idx].obj_ptr = obj_ptr;
    s_atexit_funcs[idx].dso_handle = dso_handle;

    return 0;
}

extern "C" void __cxa_finalize(void* f)
{
    if (!f)
    {
        for (unsigned int idx = s_atexit_funcs_count; idx--;)
        {
            if (s_atexit_funcs[idx].destructor_func)
            {
                (*s_atexit_funcs[idx].destructor_func)(s_atexit_funcs[idx].obj_ptr);
            }
        }
    }
    else
    {
        for (unsigned int idx = s_atexit_funcs_count; idx--;)
        {
            if (s_atexit_funcs[idx].destructor_func == f)
            {
                (*s_atexit_funcs[idx].destructor_func)(s_atexit_funcs[idx].obj_ptr);
                s_atexit_funcs[idx].destructor_func = 0;
            }
        }
    }
}


extern "C" void __cxa_pure_virtual()
{
    monitor::Print("__cxa_pure_virtual was called!");
}