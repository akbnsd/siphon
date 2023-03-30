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

#ifndef LINK_HPP
#define LINK_HPP


#include "addr.hpp"
/**
 * @brief link manages connections with the remote hosts
 */
class link
{
public:

    enum flags{
        IPV4 = 0x00,
        IPV6 = 0x01,
        TCP  = 0x00,
        UDP =  0x02
    };


    typedef bool (*requestHandler)(); 

    link(int port, int flg = IPV4 | TCP);

    bool send(char* data, int length, addr adr = addr());
    // TODO: overload send
    // inline bool send(std::string& data, addr& adr) { return send((char*)data.c_str(), data.length(), adr); }

    int recv(char* data, int size);
    int recv(char* data, int size, addr& recvAddr);
    // TODO: overload recv
    // int recv(std::string* writeTo, addr& recvAddr);

    bool listen(int backlog = 5);


    bool accept(link& lnk);

    bool connect(addr adr);

    inline bool isValid() { return fd != -1;}
    inline const addr& getRemoteAddr() { return remote; }
    void shut();

    ~link();

private:
    int fd=-1;
    bool ipv4=true, tcp=true;
    addr remote;


    

};


#endif // LINK_HPP
