"""."""


# NOTE: Struct
class Token:
    def __init__(self, type, content):
        self.type = type
        self.content = content

    def __repr__(self):
        return f"[{self.type}:{self.content}]"


class Lexer:
    """."""
    ERR = -1
    ACP = 99

    #     *   ' '  0-9   .    M   A^_  < >   !    =   ( )   &    |   _^A
    #     0    1    2    3    4    5    6    7    8    9    10   11   12  
    transitions = [
        [ERR,   0,   1,   2,   3,   4,   5,   7,   8,   9,  10,  11,  12],  # 0
        [ERR, ACP,   1,   2, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP],  # 1 Number
        [ERR, ACP,   2, ERR, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP],  # 2 Number
        [ERR, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP],  # 3 Arithmetic Operator
        [ERR, ACP,   4, ERR, ACP,   4, ACP, ACP, ACP, ACP, ACP, ACP, ACP],  # 4 Symbol
        [ERR, ACP, ACP, ERR, ACP, ACP, ACP, ACP,   6, ACP, ACP, ACP, ACP],  # 5 Comparison Operator
        [ERR, ACP, ACP, ERR, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP],  # 6 Comparison Operator
        [ERR, ACP, ACP, ERR, ACP, ACP, ACP, ACP,   6, ACP, ACP, ACP, ACP],  # 7 Logical Operator
        [ERR, ACP, ACP, ERR, ACP, ACP, ACP, ACP,   6, ACP, ACP, ACP, ACP],  # 8 Assigment Operator
        [ERR, ACP, ACP, ERR, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP],  # 9 Delimitator
        [ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR,  12, ERR, ACP],  # 10 &
        [ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR, ERR,  12, ACP],  # 11 |
        [ERR, ACP, ACP, ERR, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP],  # 12 Logical Operator
        [ERR, ACP,  12, ERR, ACP, ACP, ACP, ACP, ACP, ACP, ACP, ACP,  12]   # 13 Symbol
    ]

    default = 0
    columns = [
         (1, lambda c: c in [' ', '\n', '\t']),
         (2, lambda c: c.isdigit()),
         (3, lambda c: c == '.'),
         (4, lambda c: c in '+-*/%^'),
         (5, lambda c: c.isalpha() or c == '_'),
         (6, lambda c: c in '<>'),
         (7, lambda c: c == '!'),
         (8, lambda c: c == '='),
         (9, lambda c: c in "([{}])"),
        (10, lambda c: c == '&'),
        (11, lambda c: c == '|'),
        (12, lambda c: c.isalpha())
    ]

    types = {
         0: 'ERR_0',
         1: 'Number',
         2: 'Number',
         3: 'Operator',
         4: 'Symbol',
         5: 'Comparison Operator',
         6: 'Comparison Operator',
         7: 'Logical Operator',
         8: 'Assigment Operator',
         9: 'Delimitator',
        12: 'Logical Operator',
        13: 'Symbol'
    }

    def __init__(self, expression):
        self.expression = expression

    def __iter__(self):
        output = []
        token = ''
        lexem = ""

        prev_state = 0
        idx = 0
        while idx < len(self.expression):
            token = self.expression[idx]
            column = self._get_column(token)
            state = self.transitions[prev_state][column]

            if state == self.ACP:
                token_type = self.types[prev_state]
                yield Token(token_type, lexem)
                lexem = ""

                idx -= 1
                state = 0

            elif state == self.ERR:
                print("Error with lexem:", lexem)
                state = 0

            else:
                if token not in [' ', '\n', '\t']:
                    lexem += token

            idx += 1
            prev_state = state


    def _get_column(self, c):
        for column in self.columns:
            if column[1](c):
                return column[0]

        return self.default
