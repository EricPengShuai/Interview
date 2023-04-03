#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

class MemoryBlock
{
public:

    // Simple constructor that initializes the resource.
    explicit MemoryBlock(size_t length)
            : _length(length)
            , _data(new int[length])
    {
        std::cout << "In MemoryBlock(size_t). length = "
                  << _length << "." << std::endl;
    }

    // Destructor.
    ~MemoryBlock()
    {
        std::cout << "In ~MemoryBlock(). length = "
                  << _length << ".";

        if (_data != nullptr)
        {
            std::cout << " Deleting resource.";
            // Delete the resource.
            delete[] _data;
        }

        std::cout << std::endl;
    }

    // Copy constructor.
    MemoryBlock(const MemoryBlock& other)
            : _length(other._length)
            , _data(new int[other._length])
    {
        std::cout << "In MemoryBlock(const MemoryBlock&). length = "
                  << other._length << ". Copying resource." << std::endl;

        std::copy(other._data, other._data + _length, _data);
    }

    // Copy assignment operator.
    MemoryBlock& operator=(const MemoryBlock& other)
    {
        std::cout << "In operator=(const MemoryBlock&). length = "
                  << other._length << ". Copying resource." << std::endl;

        if (this != &other)
        {
            // Free the existing resource.
            delete[] _data;

            _length = other._length;
            _data = new int[_length];
            std::copy(other._data, other._data + _length, _data);
        }
        return *this;
    }

    // Move constructor.
    MemoryBlock(MemoryBlock&& other) noexcept
            : _data(nullptr)
            , _length(0)
    {
        std::cout << "In MemoryBlock(MemoryBlock&&). length = "
                  << other._length << ". Moving resource." << std::endl;

        // Copy the data pointer and its length from the source object.
        _data = other._data;
        _length = other._length;

        // Release the data pointer from the source object so that
        // the destructor does not free the memory multiple times.
        other._data = nullptr;
        other._length = 0;

        // 以上语句与直接使用 move 等效
        // *this = std::move(other);
    }

    // Move assignment operator.
    MemoryBlock& operator=(MemoryBlock&& other) noexcept
    {
        std::cout << "In operator=(MemoryBlock&&). length = "
                  << other._length << "." << std::endl;

        if (this != &other)
        {
            // Free the existing resource.
            delete[] _data;

            // Copy the data pointer and its length from the source object.
            _data = other._data;
            _length = other._length;

            // Release the data pointer from the source object so that
            // the destructor does not free the memory multiple times.
            other._data = nullptr;
            other._length = 0;
        }
        return *this;
    }

    // Retrieves the length of the data resource.
    size_t Length() const
    {
        return _length;
    }

private:
    size_t _length; // The length of the resource.
    int* _data; // The resource.
};

int main()
{
    string s = "test";
    string&& r = std::move(s);   // 将左值 s 转成右值引用 r
    cout << s << ' ' << r << endl;  // test test

    string t(r);    // [拷贝构造函数] r 这里又被转成左值，依旧没有移走 s
    cout << s << ' ' << r << ' ' << t << endl;  // test test test

    string x(std::move(r));  // [移动构造函数] 此时 s 和 r 才被转移
    cout << s << ' ' << r << ' ' << x << endl;  // 此时 s 和 r 都为空

    string y = "test";
    string z(std::move(y));  // 触发移动构造函数转移 y
    cout << y << ' ' << z << endl;  // y = "", z = "test"


    // 深入理解 std::move
    // Create a vector object and add a few elements to it.
    vector<MemoryBlock> v;
    MemoryBlock b(25);
    v.push_back(b); // 调用复制构造函数
    v.push_back(std::move(b));   // 调用转移构造函数

    return 0;
}