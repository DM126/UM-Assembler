#include <cstdint>
#include <stack>
#include <string>

struct Token
{
    bool isImmediate; //True if immediate, false if register
    uint32_t value; //immediate value or register number
}

class Assembler
{
public:
    Assembler(const string& filename);
private:
    void parseLine();
};
