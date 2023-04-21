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


#ifdef _WIN32
// #error WINDOWS IMPLEMENTATION NOT DEFINED
#include "pch.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

#include "link.hpp"

bool WS_NOT_INIT = true;

bool ws_INIT()
{

    WSADATA wsaData;
    // Initialize Winsock
    WS_NOT_INIT = WSAStartup(MAKEWORD(2,2), &wsaData);
    return WS_NOT_INIT;
};


link::link(int port, int flag)
{
    if(WS_NOT_INIT) ws_INIT();
    
    ipv4 = ! (flag & IPV6);
    tcp = !  (flag & UDP);

    unsigned long one = 1;
    int err = 0;
    fd = socket( 
        ipv4 ? AF_INET : AF_INET6,
        (tcp  ? SOCK_STREAM : SOCK_DGRAM),
        0);

    if(fd == INVALID_SOCKET) return;

    err = ioctlsocket(fd, FIONBIO, &one);

    sockaddr_in in = {};
    in.sin_family = AF_INET;
    in.sin_port = htons(port);
    in.sin_addr.S_un.S_addr = INADDR_ANY;

    sockaddr_in6 in6 = {};
    in6.sin6_family = AF_INET6;
    in6.sin6_port = htons(port);
    in6.sin6_addr = in6addr_loopback;

    err = bind(fd, 
        (ipv4) ? (sockaddr*) &in : (sockaddr*) &in6, 
        (ipv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6));
    if(err)
    {
        err = WSAGetLastError();
        shutdown(fd, SD_BOTH);
        fd =-1;
        return;
    }
};


bool link::send(char *data, int length, addr adr)
{
    adr.update();
    char* end = data + length;
    int adrLen = (ipv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    int ret=0;
    while (data < end)
    {
        ret = sendto(fd, data, length, 0, (sockaddr*) adr.getNAddr(), adrLen);
        if(ret == SOCKET_ERROR) return false;
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
    int len = (ipv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
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

bool link::connect(addr adr)
{
    adr.update();
    bool status = !::connect(fd, (sockaddr*)adr.getNAddr(), (ipv4) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6));
    if (!status)
    {
        int err = WSAGetLastError();
        status = (err == WSAEWOULDBLOCK);
    }
    return status;
}

void link::shut()
{
    if(fd != -1) shutdown(fd, SD_BOTH);
}

link::~link()
{
    shut();
}
#endif
