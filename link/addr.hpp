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

#ifndef ADDR_HPP
#define ADDR_HPP


#include "pch.hpp"

// ipv6 address struct size
#define MAX_ADDRESS_SIZE 28

/**
 * @brief addr manages ip addresses
 */
struct addr
{
    /**
     * @brief port and ip of host
     * port can range from 1 to 65535
     * ip can be of IPV4 or IPV6 in presentation format as shown.
     * IPV4  ->    x.x.x.x      where  0 =< x =< 255
     * IPV6  ->    xxxx.xxxx.xxxx.xxxx      where  x is in hexadecimal notation
     * call update() when either of them is changed to update native data.
     */
    int port=0;
    std::string ip="";

    /** @brief updates host details in native dataset
     * @param reverse if set to true, the ip is updated from native dataset
     */
    void update(bool reverse=false);

    /**
     * @brief 
     * overloaded operators  <<, so that we can do this
     * addr << "127.0.0.1" << 8080;
     */
    const addr& operator<<(std::string ip);
    const addr& operator<<(int port);

    //getNative address access 
    inline void* getNAddr() { return n_addr; };

    inline bool isIPV4() { return IPV4; }

    friend class socket;

private:

    // stores data in network notation
    char n_addr[MAX_ADDRESS_SIZE];
    // flags to save address type
    bool IPV4=true;
};


#endif //  ADDR_HPP
