// This file is a part of  siphon
// Copyright (C) 2023  akshay bansod <aksh.bansod@yahoo.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifdef _LINUX

#include <netinet/in.h>
#include <arpa/inet.h>

#include "link.hpp"



link::link(int port, int flg)
{
    ipv4 = ! (flg & IPV6);
    tcp = !  (flg & UDP);

    fd = socket( 
        ipv4 ? AF_INET : AF_INET6,
        (tcp  ? SOCK_STREAM : SOCK_DGRAM) | SOCK_NONBLOCK,
        0);

    if(fd == -1) return;

    sockaddr_in in{
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = INADDR_ANY
    };

    sockaddr_in6 in6;
    in6.sin6_family = AF_INET6;
    in6.sin6_port = htons(port);
    in6.sin6_addr = in6addr_loopback;

    if(bind(fd, 
        (ipv4) ? (sockaddr*) &in : (sockaddr*) &in6, 
        (ipv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6)) == -1)
    {
        shutdown(fd, SHUT_RDWR);
        fd =-1;
        return;
    }
}

bool link::send(char *data, int length, addr adr)
{
    adr.update();
    char* end = data + length;
    int adrLen = (ipv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    int ret=0;
    while (data < end)
    {
        ret = sendto(fd, data, length, 0, (sockaddr*) adr.getNAddr(), adrLen);
        if(ret == -1) return false;
        data += ret;
        length -= ret;
    }

    return true;
}

int link::recv(char *data, int size)
{
    return ::recv(fd, data, size, 0);
}

int link::recv(char *data, int size, addr &recvAddr)
{
    unsigned int len=0;
    int ret=0;

    ret = recvfrom(fd, data, size, 0, (sockaddr*) recvAddr.getNAddr(), &len);
    recvAddr.update(true);
    recvAddr.IPV4 = ipv4;
    return ret;
}

bool link::listen(int backlog)
{
    return ! ::listen(fd, backlog);
}

bool link::accept(link& child)
{
    child = *this;
    socklen_t len= sizeof(sockaddr_in6);
    child.fd = ::accept(fd, (sockaddr*) child.remote.getNAddr(), &len);
    if( child.fd == -1) return false;
    child.remote.update(true);
    return true;
}

bool link:: connect(addr adr)
{
    adr.update();
    return ::connect(fd, (sockaddr*) adr.getNAddr(), (ipv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6)) != -1;
}

void link::shut()
{
    if(fd != -1) shutdown(fd, SHUT_RDWR);
}

link::~link()
{
    shut();
}

#endif
