#include "pch.h"
#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <chrono>
#include "corefunc.h"
#include "userfunc.h"
#include <string>
#include <direct.h>

using namespace std;

vector<thread> threads;

void botxd() {
    GrowtopiaBot bot;
    bot.userInit();
    bot.eventLoop();
    bot.userLoop();

    /*
         * These crash methods found by me @batutekoz77 (github).
         * These methods will work on GTPS3 base Sources (if not dialogs fully fixed).
    */
}

int main() {
    init();

    /*
         * Connecting loop to thread method.
    */

    for(int i = 0;i < 100;i++) threads.push_back(std::thread(botxd));
    
    while(true) {}
}
