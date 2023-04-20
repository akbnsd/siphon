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


// windows implementation for addr methods

#ifdef _WIN32

#include "pch.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

#include "addr.hpp"


void addr::update(bool reverse)
{
    valid = true;
    // update ip and port for network format
    if(reverse){
        char address[40];
        sockaddr_in6* in6 = (sockaddr_in6*) n_addr;

        if(IPV4){
            sockaddr_in* in = (sockaddr_in*) n_addr;
            valid &= (inet_ntop( AF_INET, (void*) &in->sin_addr.s_addr, address, 40) != 0);
            port = ntohs(in->sin_port);
        }
        else{
            sockaddr_in6* in = (sockaddr_in6*) n_addr;
            valid &= (inet_ntop( AF_INET6, (void*) &in->sin6_addr, address, 40) != 0);
            port = ntohs(in->sin6_port);
        }

        ip = address;
        return;
    }
    else if(ip == "" && port <= 0) {
        valid = false;
        return;
    }

    // if not reverse and ip and port are valid
    sockaddr_in in;
    sockaddr_in6 in6;

    // check whether ip of IPV4 or IPV6
    IPV4 = (inet_pton(AF_INET, ip.c_str(), &in.sin_addr)) ? 
        true : inet_pton(AF_INET6, ip.c_str(), &in6.sin6_addr) ? 
            false : valid = false;

    if(IPV4){
        in.sin_family = AF_INET;
        in.sin_port = htons(port);

        memcpy(n_addr, &in, sizeof(in));
    }else{
        in6.sin6_family = AF_INET6;
        in6.sin6_port = htons(port);
        memcpy(n_addr, &in6, sizeof(in6));
    };
};

#endif
