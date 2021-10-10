#include "epoller.h"

Epoller::Epoller(int maxEvent) : _epollFd(epoll_create(512)), _events(maxEvent) {
    assert(_epollFd >= 0 && _events.size() > 0);
}

Epoller::~Epoller() {
    close(_epollFd);
}

bool Epoller::AddFd(int fd, uint32_t events) {
    if (fd < 0) return false;
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = events;
    return epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) == 0;
}

bool Epoller::ModFd(int fd, uint32_t events) {
    if (fd < 0) return false;
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = events;
    return epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev) == 0;
}

bool Epoller::DelFd(int fd) {
    if (fd < 0) return false;
    epoll_event ev = {0};
    return epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, &ev) == 0;
}

// 返回发生事件的数量
int Epoller::Wait(int timeoutMS) {
    return epoll_wait(_epollFd, &_events[0], static_cast<int>(_events.size()), timeoutMS);
}

int Epoller::GetEventFd(size_t i) const {
    assert(i < _events.size() && i >= 0);
    return _events[i].data.fd;
}

// 返回事件类型
uint32_t Epoller::GetEvents(size_t i) const {
    assert(i < _events.size() && i >= 0);
    return _events[i].events;
}