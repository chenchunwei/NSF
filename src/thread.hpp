#ifndef __NSF_THREAD_HPP_INCLUDED__
#define __NSF_THREAD_HPP_INCLUDED__

#include "platform.hpp"

#ifdef NSF_WINDOWS
#include "windows.hpp"
#else
#include <pthread.h>
#endif

namespace nsf
{

    typedef void (thread_fn) (void*);

    class thread_t
    {
    public:
        inline thread_t (){}

        void start(thread_fn *tfn_, void *arg_);
        void stop();

        thread_fn *tfn;
        void *arg;
 
    private:

#ifdef NSF_WINDOWS
        HANDLE descriptor;
#else
        pthread_t descriptor;
#endif

        thread_t(const thread_t&);
        const thread_t &operator = (const thread_t&);
    };
}

#endif
