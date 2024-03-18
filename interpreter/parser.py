"""."""


class Parser:
    def __init__(self, lexer):
        self.stack = []
        self.symbol_table = {}
        self.lexer = iter(lexer)

    # NOTE: Axioma
    def __call__(self):
        try:
            self.expression()

        except StopIteration:
            print("Compilacion Finalizada")

    def _error(self, expected, obtained):
        print(f"Syntax Error: '{expected}' was expected but {obtained} was received")
        
    def expresion(self):
        self.expr_0()

    def expr_0():
        pass

    def term(self):
        token = next(self.lexer)
        if token.lexem == '(':
            self.expresion()

            token = next(self.lexer)
            if token.lexem != ')':
                self._error(')', token.lexem)

        if token.type not in ['Number', 'Symbol']:
            self._error('Number or Symbol', token.type)
