#include <iostream>
using namespace std;

struct Storage {
    char      a;    // 1
    int       b;    // 4
    double    c;    // 8
    long long d;    // 8
};

// 其中 max_align_t 要求每个标量类型的对齐方式严格一样，因此它几乎是最大标量没有差异
// 进而大部分平台上得到的结果为 long double，因此我们这里得到的 AlignasStorage 的对齐要求是 8 或 16
struct alignas(max_align_t) AlignasStorage {
    char      a;
    int       b;
    double    c;
    long long d;
};

int main() {
    cout << alignof(Storage) << endl;
    cout << alignof(AlignasStorage) << endl;
    return 0;
}
