#include <windows.h>
#include <iostream>
#include <cstdlib>

using namespace std;

void readprocess_errorMsg(void);

int main(int argc, char **argv)
{
    DWORD gold_value = 0;
    DWORD new_gold_value = 10000;
    DWORD bytes_read = 0;
    DWORD process_id = 0;
    DWORD bytes_written = 0;
    DWORD gld_val = 0;

    //if lpClassName is Null, it finds title that matches the lpWindowName param
    HWND window_handle = FindWindowA(NULL, "The Battle for Wesnoth - 1.14.9");
    if (window_handle == NULL)
    {
        cout<< "Window Handle not found!\n";
        system("pause");
        exit(99);
    }
    cout << "Window Handle Successful: " << window_handle;

    //window_handle is obtained from the HWND FindWindowA
    int _ThreadID = GetWindowThreadProcessId(window_handle, &process_id);
    if (_ThreadID == 0)
    {
        cout<< "Thread Process Id not found!\n";
        system("pause");
        exit(99);
    }
    cout << "\nThread Process Id Successful: " << _ThreadID;

    //targetProcessId is obtained from the GetWindowThreadProcessId API
    HANDLE wesnoth_process = OpenProcess(PROCESS_ALL_ACCESS, TRUE, process_id);
    if (wesnoth_process == NULL)
    {
        cout << "\nWesnoth_process not found!\n";
        system("pause");
        exit(99);
    }
    cout << "\nWesnoth_process Successful: " << wesnoth_process;

    //wesnoth_process is obtained from the OpenProcess API
    //base pointer is 0x017EECB8, we add 0x60 then 0xA90 and offset by 4 to find exact
    //gold value
    int rd_pNo = ReadProcessMemory(wesnoth_process, (void*)0x017EED18, &gold_value, 4, &bytes_read);
    if (rd_pNo == 0)
        readprocess_errorMsg();
    cout << "\nFirst Read Process Successful!";

    DWORD sec_p_addr = gold_value + 0xA90;

    rd_pNo = ReadProcessMemory(wesnoth_process, (void*)sec_p_addr, &gold_value, 4, &bytes_read);
    if (rd_pNo == 0)
        readprocess_errorMsg();
    cout << "\nSecond Read Process Successful";
cout << "\nGold value is: " << gold_value <<"\n";

    DWORD trd_p_addr = gold_value + 4;

    rd_pNo = ReadProcessMemory(wesnoth_process, (void*)gold_value, &gold_value, 4, &bytes_read);
    if (rd_pNo == 0)
        readprocess_errorMsg();
    cout << "\nThird Read Process Successful";
    cout << "\n\nInitial Gold Value is: " << gold_value << "\n\n";

    int wrt = WriteProcessMemory(wesnoth_process, (void*)trd_p_addr, &new_gold_value, 4, &bytes_written);
    if (wrt == 0)
    {
          cout << "\n\nERROR: Write Process Failed!\n";
          int api_err = GetLastError();
          cout << "Error No.: " << api_err << "\n\n";
          system("pause");
          exit(99);
    }

    int rd_pNo1 = ReadProcessMemory(wesnoth_process, (void*)gold_value, &gld_val, 4, &bytes_read);
    if (rd_pNo1 == 0)
        readprocess_errorMsg();

    cout<< "\nNew Gold Value is: "<< gld_val;

    return (0);
}

void readprocess_errorMsg(void)
{
        cout << "\n\nERROR: Read Process Failed!\n\n";
        int api_err = GetLastError();
        cout << "Error No.: " << api_err << "\n";
        system("pause");
        exit(98);
}
