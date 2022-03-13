#include <iostream>
using namespace std;

int main() {
//    const int local = 10; // 10 10
//    volatile const int local = 10;  // 10 100
    const volatile int local = 10;  // 10 100

    int *ptr = (int*) &local;
    printf("Initial value of local: %d \n", local);

    *ptr = 100;
    printf("Modified value of local: %d \n", local);
    return 0;
}
