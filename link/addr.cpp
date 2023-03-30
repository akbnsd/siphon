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
