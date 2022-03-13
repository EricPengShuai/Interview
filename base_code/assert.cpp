#define NDEBUG
#include <cassert>

using namespace std;

int main() {
    int x = 7;
    x = 9;
    assert(x == 7);
    return 0;
}
