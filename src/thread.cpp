#include "platform.hpp"
#include "thread.hpp"

#ifdef NSF_WINDOWS

extern "C"
{
    static unsigned int __stdcall thread_routine (void *arg_)
    {
        nsf::thread_t *self = (nsf::thread_t*) arg_;
        self->tfn (self->arg);
        return 0;
    }
}

void nsf::thread_t::start (thread_fn *tfn_, void *arg_)
{
    tfn = tfn_;
    arg =arg_;
    descriptor = (HANDLE) _beginthreadex (NULL, 0,
        &::thread_routine, this, 0 , NULL);
    //win_assert (descriptor != NULL);
}

void nsf::thread_t::stop ()
{
    DWORD rc = WaitForSingleObject (descriptor, INFINITE);
    //win_assert (rc != WAIT_FAILED);
    BOOL rc2 = CloseHandle (descriptor);
    //win_assert (rc2 != 0);
}

#else

#include <signal.h>

extern "C"
{
    static void *thread_routine (void *arg_)
    {
#if !defined ZMQ_HAVE_OPENVMS
        //  Following code will guarantee more predictable latecnies as it'll
        //  disallow any signal handling in the I/O thread.
        sigset_t signal_set;
        int rc = sigfillset (&signal_set);
        errno_assert (rc == 0);
#   if !defined ZMQ_HAVE_ANDROID
        rc = pthread_sigmask (SIG_BLOCK, &signal_set, NULL);
        posix_assert (rc);
#   endif
#endif

        zmq::thread_t *self = (zmq::thread_t*) arg_;
        self->tfn (self->arg);
        return NULL;
    }
}

void nsf::thread_t::start (thread_fn *tfn_, void *arg_)
{
    tfn = tfn_;
    arg =arg_;
    int rc = pthread_create (&descriptor, NULL, thread_routine, this);
    posix_assert (rc);
}

void nsf::thread_t::stop ()
{
    int rc = pthread_join (descriptor, NULL);
    posix_assert (rc);
}

#endif