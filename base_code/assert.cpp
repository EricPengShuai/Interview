#define NDEBUG  // assert 只有在Debug版本中才有效，因此如果加这句话会忽略 assert 语句
#include <cassert>

using namespace std;

int main() {
    int x = 7;
    x = 9;
    assert(x == 7);
    return 0;
}
