#include <iostream>

using namespace std;

int main() {
    char buf[] = "#", *ptr = buf;
    while (*ptr != '#') {
        S0:
        while (*ptr == 'b') ptr++;
        S1:
        while (*ptr == 'a') ptr++;
        switch (*ptr) {
            case 'b':
                ptr++;
                switch (*ptr) {
                    case 'a':
                        goto S1;
                    case 'b':
                        ptr++;
                        switch (*ptr) {
                            case 'a':
                                goto S1;
                            case 'b':
                                goto S0;
                            case '#':
                                cout << "yes" << endl;
                                return 0;
                            default:
                                break;

                        }
                    default:
                        break;
                }
            default:
                break;
        }
        break;
    }
    cout << "no" << endl;
    return 0;
}