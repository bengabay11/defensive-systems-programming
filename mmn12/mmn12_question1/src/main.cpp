#include <iostream>
#include <string>

int get_credit(int ID) {
    return -100; // Set this to use the type conversion vulnerability in `vulnerable_is_entitled_for_promotional_gift`
}

bool fix_is_entitled_for_promotional_gift(int ID)
{
    int bound = 750;
    int credit = get_credit(ID);
    return (credit >= bound);
}

bool vulnerable_is_entitled_for_promotional_gift(int ID)
{
    unsigned int bound = 750;
    int credit = get_credit(ID);
    return (credit >= bound);
}

int main() {
    int id = 6;
    bool vulnerable_result = vulnerable_is_entitled_for_promotional_gift(id);
    bool fix_result = fix_is_entitled_for_promotional_gift(id);
    std::cout << "Vulnerable 'is_entitled_for_promotional_gift' returned - "<< vulnerable_result << std::endl;
    std::cout << "Fixed 'is_entitled_for_promotional_gift' returned - " << fix_result << std::endl;
}
