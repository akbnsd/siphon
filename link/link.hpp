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
 * @mainpage Networking Library
 *  @section Intro Introduction
 * Siphon is a networking library for quick networking setup. \n
 * Currently siphon supports
 *  - TCP / UDP   protocols
 *  - IPV4 / IPV6 addressing
 *  - Linux platform
 * 
 * The library contains 
 * - @ref link which handles the connection
 * - @ref addr which handles addressing and translations
 * 
 * @section workflow WorkFlow
 * The following is the overall workflow of the library.
 * @subsection UDP
 * - construct link object with UDP as one of the flags
 *      >  link lnk(500, IPV4 | UDP );
 * - send / recieve data
 *      >  addr adr; \n
 *      >  adr.ip = "127.0.0.1"; \n
 *      >  adr.port = 80;  \n
 *      >
 *      >  lnk.send(data, 100, adr); \n
 *      >  lnk.recv(data, 100, adr);
 * 
 * - close the link
 *      > lnk.shut();
 * 
 * 
 * @subsection tcp_client TCP client
 * 
 * - construct link object with TCP as one of the flags
 *      >  link lnk(500, IPV4 | TCP );
 * - attempt to connect
 *      >  addr adr; \n
 *      >  adr.ip = "127.0.0.1"; \n
 *      >  adr.port = 80; 
 *      >
 *      > while(!lnk.connect(adr)){};
 * 
 * - send / recieve data on a successful connection
 *      >  lnk.send(data, 100, adr); \n
 *      >  lnk.recv(data, 100, adr);
 * 
 * - close the link
 *      > lnk.shut();
 * 
 * @subsection tcp_server TCP server
 * 
 * - construct link object with TCP as one of the flags
 *      >  link lnk( 80, IPV4 | TCP ); \n
 *      >  addr adr;
 * - begin listening
 *      > listen(5);
 * - try accepting connection
 *      > while(!lnk.accept(adr)){};
 * 
 * - send / recieve data on a successful connection
 *      >  lnk.send(data, 100, adr); \n
 *      >  lnk.recv(data, 100, adr);
 * 
 * - close the link
 *      > lnk.shut();
 * 
 */





/**
 * @brief link manages connections with the remote hosts.
 * 
 * @details
 * More setup can be needed to begin packet exchange, depending upon the protocol. \n
 * after constructing link follow is setup is required to begin communication.
 * udp -  no setup required. Can directly start comm. with send() recv() while specifying the address. \n
 *  
 */
class link
{
public:

    /**
     * @brief flags specify link properties.
     * Just bitwise OR the required configuration and pass to link constructor. \n
     * 
     * @details
     * for ex: \n
     *      IPV6 | UDP    for datagram over ipv6 addressing \n
     *      IPV4 | TCP    for TCP/IP communication  \n 
     */
    enum flags{
        IPV4 = 0x00 << 0,
        IPV6 = 0x01 << 0,
        TCP  = 0x00 << 1,
        UDP =  0x01 << 1
    };


    /**
     * @brief Construct a new link object
     * 
     * @param port localhost port to bind
     * @param flg configuration flags. see link::flags
     * 
     * @details
     * The constructor creates a bound socket of required address format and protocol.
     */
    link(int port, int flg = IPV4 | TCP);

    /**
     * @brief send data over the communcation line
     * 
     * @param data pointer to data
     * @param length  size of the data to send in bytes
     * @param adr    address of the remote host. Required only in UDP link
     * @return true if data was send successfully
     * @return false if transaction failed
     * 
     * @details
     *  Reads 'length' bytes of data from buffer and writes to the socket. The data could be segmented into multiple
     *  packets while sending if length exceeds packetSize.
     */
    /// @todo: overload send
    bool send(char* data, int length, addr adr = addr());
    // inline bool send(std::string& data, addr& adr) { return send((char*)data.c_str(), data.length(), adr); }


    /**
     * @brief reads data from the communication link and writes to memory
     * 
     * @param buffer pointer to buffer to write recieved data
     * @param size size of the buffer
     * @return int number of bytes written in buffer.
     */
    int recv(char* buffer, int size);

    /**
     * @copybrief recv()
     * 
     * @param buffer pointer to buffer to write recieved data
     * @param size size of the buffer
     * @param recvAddr addr reference to write reception address into
     * @return int number of bytes written in buffer.
     * 
     * @details
     * recieves data as recv() but stores the recieved packet address into the recvAddr.
     */
    int recv(char* buffer, int size, addr& recvAddr);
    // int recv(std::string* writeTo, addr& recvAddr);


    /**
     * @brief start listening for incoming connection requests. use only in TCP type link.
     * 
     * @param backlog no of connection to keep on hold
     * @return true if listening initiated
     * @return false if error 
     * 
     * @details
     * used only in tcp mode. Starts listening for incoming TCP Connection requests. Requests could be accepted
     * by accept().
     */
    bool listen(int backlog = 5);

    /**
     * @brief accepts a pending TCP connection request
     * 
     * @param lnk link to handle the new connection
     * @return true if connection was availabel and accepted
     * @return false if no connection request
     * 
     * @details
     * accepts TCP connection request and fills data into lnk. lnk now handles the transactions with the host.
     * the remote address can be querried through getRemoteAddr().
     * 
     */
    bool accept(link& lnk);

    /**
     * @brief send a TCP connection request to the given address.
     * 
     * @param adr address to send request
     * @return true if request is accepted by the remote host
     * @return false if request denied or host unavailable
     */
    bool connect(addr adr);

    /**
     * @brief check whether if the  connection is active
     * 
     * @return true if connection is active and no errors occured
     * @return false if connection close or error occured
     */
    inline bool isValid() { return fd != -1;}

    /**
     * @brief Get the Remote Addr object
     * 
     * @return const addr& of the remote host address.
     * @details
     * The remote address is required if the connection was accepted from accept().
     */
    inline const addr& getRemoteAddr() { return remote; }


    /// @brief  close the connection and destroy the socket
    void shut();

    /// @brief closes the connection on destruction
    ~link();

private:
    int fd=-1;
    bool ipv4=true, tcp=true;
    addr remote;

    /// @todo: add thread splitting for accepting connections
    // typedef bool (*requestHandler)(); 
};


#endif // LINK_HPP
