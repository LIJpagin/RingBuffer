#include <thread>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <windows.h>
#include "RingBuffer.h"

constexpr int SIZE_BUFFER = 20;

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);
    COORD cursorPosition = { 0, 0 };

    rb::RingBuffer<char, SIZE_BUFFER> buffer;

    auto task_for_stream = [&buffer](char beg, char end, int sleep) {
        char ch = beg;
        while (true) {
            buffer.push_back(ch);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
            ch = (ch == end) ? beg : ++ch;
        }
    };

    std::thread t1 (task_for_stream, 'a', 'z', 500);
    std::thread t2 (task_for_stream, 'A', 'Z', 1000);
    std::thread t3 (task_for_stream, '0', '9', 1500);

    while (true) {
        SetConsoleCursorPosition(hConsole, cursorPosition);
        std::for_each(buffer.end() - buffer.size() + 1, buffer.end(), [](char ch) { std::cout << ch; });
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    t1.join();
    t2.join();
    t3.join();

    return 0;
}