"""Ejecutamos el programa."""


import sys
from lexer import Lexer


def main():
    if len(sys.argv) > 1:
        tokenize_all_files()

    else:
        interactive()


def tokenize_all_files():
    """Change depending if program has file input."""
    for file in sys.argv[1:]:
        lexer = tokenize_file(file)
        print(f"##### {file} #####")
        for token in lexer:
            print(token)


def tokenize_file(path):
    """Get a file path from console."""
    with open(path, 'r') as file:
        data = file.read()

    data += ' '
    lexer = Lexer(data)
    return lexer


def interactive():
    """Simple read of user input."""
    print("Escribe una entrada. Escape en linea vacia para salir:")
    texto = ""
    entrada = input("> ")
    
    while entrada != '':
        texto += entrada + '\n'
        entrada = input("| ")

    print()
    texto += ' '
    lexer = Lexer(texto)
    for token in lexer:
        print(token)


if __name__ == '__main__':
    main()
