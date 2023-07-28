#include <windows.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    DWORD gold_value = 0;
    DWORD new_gold_amount = 10000000;
    DWORD bytes_read = 0;
    DWORD process_id = 0;
    DWORD bytes_written = 0;

    //if lpClassName is Null, it finds title that matches the lpWindowName param
    HWND window_handle = FindWindowA(NULL, "The Battle for Wesnoth - 1.14.9");

    //window_handle is obtained from the HWND FindWindowA
    GetWindowThreadProcessId(window_handle, &process_id);

    //targetProcessId is obtained from the GetWindowThreadProcessId API
    HANDLE wesnoth_process = OpenProcess(PROCESS_ALL_ACCESS, TRUE, process_id);

    //wesnoth_process is obtained from the OpenProcess API
    //base pointer is 0x017EECB8, we add 0x60 then 0xA90 and offset by 4 to find exact
    //gold value
    ReadProcessMemory(wesnoth_process, (void*)0x017EED18, &gold_value, 4, &bytes_read);

    gold_value += 0xA90;

    ReadProcessMemory(wesnoth_process, (void*)gold_value, &gold_value, 4, &bytes_read);

    gold_value += 4;

    WriteProcessMemory(wesnoth_process, (void*)gold_value, &new_gold_amount, 4, &bytes_written);

    ReadProcessMemory(wesnoth_process, (void*)gold_value, &gold_value, 4, &bytes_read);

    cout<< "Gold value: "<< gold_value<< "\nWesnoth process: " << wesnoth_process << "\nNew gold amount: "<< new_gold_amount <<endl;
    return (0);
}
