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


/**
 * @brief link manages connections with the remote hosts
 */

class link
{
public:

    /**
     * @brief constructs a locally bound port
     * 
     * @param port port to bind
     * calling the constructor creates a local bound link.
     * This should be only done once for a port. The individual connections @links should be created
     * through fork calls. 
     */
    link(int port);



    link fork();



private:
    void* mSock;
    

};
