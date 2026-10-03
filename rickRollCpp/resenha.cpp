#include <windows.h>
#include <iostream>
using namespace std;

int main() {
    int resp;
    cout << "You want to play the game? [yes = 1, no = 0]";
    cin >> resp;
    
    if (resp == 1) {
        cout << "Glad you said yes" << endl;
        for(int i = 0; i < 15; i++) {
            ShellExecuteW(NULL, L"open", L"https://www.youtube.com/watch?v=oHg5SJYRHA0", NULL, NULL, SW_SHOWNORMAL);
        }
    } else if (resp == 0) {
        cout << "Y u so no?" << endl;
    } else {
        cout << "Y u no answer correctly?!?1" << endl;
    }
    return 0;
}   