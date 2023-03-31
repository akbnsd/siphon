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

/// @brief maximum number of bytes required to store address
#define MAX_ADDRESS_SIZE 28

/**
 * @brief addr class manages ip addresses and its conversions across byte orders.
 * 
 * @details
 * set addr.ip and addr.port addr.IPV4 to requierd values. \n
 * getNAddr() returns pointer to native address.
 * 
 * what is native address? \n
 *      It is the address stored in platform specfic struct. The address is in NETWORK BITE ORDER (Big Endian)
 * 
 * \n usage   addr << "127.0.0.1" << 80;
 */
struct addr
{
    /**
     * @brief port and ip of host.
     * 
     * @details
     * port can range from 1 to 65535.
     * ip can be of IPV4 or IPV6 in presentation format as shown.
     * IPV4  ->    "x.x.x.x"      where  0 =< x =< 255
     * IPV6  ->    "xxxx.xxxx.xxxx.xxxx"      where  x is in hexadecimal notation
     * call update() when either of them is changed to update native data.
     */
    int port=0;
    /// @copydoc port
    std::string ip="";

    /// @brief set to true   if IPV4 address, false  for IPv6
    bool IPV4=true;


    /**
     * @brief 
     * overloaded operators  <<, so that we can do this. \n
     * addr << "127.0.0.1" << 8080;
     */
    const addr& operator<<(std::string ip);
    const addr& operator<<(int port);

    /**
     * @brief return native address
     * 
     * @return getNaddr() return pointer to native address
     */
    void* getNAddr();

    /// @brief checks if address is valid
    /// @return true, if valid, else false
    bool isValid();


    friend class link;
private:

    // stores data in network notation
    char n_addr[MAX_ADDRESS_SIZE];
    bool valid=false;


    /** @brief updates native address based on (string) ip and (int) port provided.
     * The native native address is used for all operations. 
     * 
     * @param reverse if set to true, the (ip,port) is updated from native address
     */
    void update(bool reverse=false);
};


#endif //  ADDR_HPP
