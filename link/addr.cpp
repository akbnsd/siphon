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



#include "addr.hpp"



const addr &addr::operator<<(std::string ipAddr)
{
    ip = ipAddr;
    if(ip != "" && port > 0) update();
    return *this;
}

const addr &addr::operator<<(int Port)
{
    port = Port;
    if(ip != "" && port > 0) update();
    return *this;
}


void *addr::getNAddr()
{
    update();
    return n_addr;
}

inline bool addr::isValid()
{
    return valid;
}
