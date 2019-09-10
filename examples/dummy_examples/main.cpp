#include <filesystem>
#include <iostream>

int main()
{
    std::cout << "Hello world\n";

    //! Check filesystem working
    std::cout << std::filesystem::current_path() << std::endl;
    return 0;
}
