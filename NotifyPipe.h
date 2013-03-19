// NotifyPipe.h
// from http://stackoverflow.com/questions/2486335/wake-up-thread-blocked-on-accept-call
#ifndef NOTIFYPIPE_H_INCLUDED
#define NOTIFYPIPE_H_INCLUDED

class NotifyPipe
{
    int m_receiveFd;
    int m_sendFd;

public:
    NotifyPipe();
    virtual ~NotifyPipe();

    int receiverFd();
    void notify();
};

#endif // NOTIFYPIPE_H_INCLUDED

