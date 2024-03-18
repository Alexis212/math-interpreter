#include <iostream>
#include <string>

void interactive();
// void tokenize_file();


int main(int argc, char *argv[])
{
    if (argc > 1)
        std::cout << "tokenize file";

    else
        interactive();
    
    return 0;
}


void interactive()
{
    std::cout << "Escribe una entrada. Escape en linea vacia para salir:\n";
    std::string text;
    std::string line;

    std::cout << "> ";
    std::getline(std::cin, line);

    while (line != "")
    {
        text += line;
        std::cout << "| ";
        std::getline(std::cin, line);
    }

    std::cout << "\n" << text;
}
