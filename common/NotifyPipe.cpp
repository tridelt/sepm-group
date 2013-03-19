// NotifyPipe.cpp
// from http://stackoverflow.com/questions/2486335/wake-up-thread-blocked-on-accept-cal
//
#include "NotifyPipe.h"

#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

NotifyPipe::NotifyPipe()
{
    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret == 0);
    m_receiveFd = pipefd[0];
    m_sendFd = pipefd[1];
}


NotifyPipe::~NotifyPipe()
{
    close(m_sendFd);
    close(m_receiveFd);
}


int NotifyPipe::receiverFd()
{
    return m_receiveFd;
}


void NotifyPipe::notify()
{
    write(m_sendFd,"1",1);
}
