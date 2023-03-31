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



#include <thread>
#include "pch.hpp"
#include "link/link.hpp"


using namespace std;

bool quit = false;

void listenAndPrint(link& lnk){
    char data[100];
    addr temp;
    int size=0;
    while (!quit)
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        size = lnk.recv(data, 100, temp);
        if(size == -1) continue;
        cout << "\n" << data << "\n                                  <<";
        memset(data, 0, 100);
    }

};
int main(){

    int task, port=0;
    cout << R"(
    tasks available:
        0 : udp send and recieve,
        1 : tcp client
        2 : tcp server

    enter task:)";

    cin >> task;
    cout << "enter host port:";
    cin >> port;


    // udp send recv
    if(task == 0){

        cout << "udp packet send and recieve mode\n";
        link lnk(port, link::IPV6 | link::UDP);
        addr adr, temp;
        cout << "enter remote address:";
        cin >> adr.ip;
        cout << "enter remote port:";
        cin >> adr.port;


        char data[100];

        std::thread lsnThrd(listenAndPrint, std::ref(lnk));
        while (data != "quit")
        {
            cout << "                                  >>";
            cin >> data;
            lnk.send(data, 100, adr);
            memset(data, 0, 100);
        }
    }



    if(task == 1){
        // tcp client
        cout << "working as tcp client\n";
        link lnk(port, link::IPV6 | link::TCP);
        addr adr, temp;
        cout << "enter remote address:";
        cin >> adr.ip;
        cout << "enter remote port:";
        cin >> adr.port;


        while(1){
            printf("trying to connect %s : %d\n", adr.ip.c_str(), adr.port);
            if(lnk.connect(adr)) break;
            this_thread::sleep_for(chrono::milliseconds(500));
        };

        cout << "connection successful !\n";

        char data[100];
        std::thread lsnThrd(listenAndPrint, std::ref(lnk));
        while (data != "quit")
        {
            cout << "                                  >>";
            cin >> data;
            lnk.send(data, 100, adr);
            memset(data, 0, 100);
        }

    };



    if(task == 2){
        // tcp server
        cout << "working as tcp server\n";
        link lnk(port, link::IPV6 | link::TCP);
        addr adr, temp;
        link child = lnk;

        lnk.listen();
        while(1){
            printf("listening for incoming connections\n");
            if(lnk.accept(child)) break;
            this_thread::sleep_for(chrono::milliseconds(500));
        };

        adr = child.getRemoteAddr();
        printf("connection request recieved from %s : %d\n", adr.ip, adr.port);


        char data[100];
        std::thread lsnThrd(listenAndPrint, std::ref(child));

        while (data != "quit")
        {
            cout << "                                  >>";
            cin >> data;
            child.send(data, 100, adr);
            memset(data, 0, 100);
        }

    };
    return 0;
};
