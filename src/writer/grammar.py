from random import choices, random
from re import compile, match


float_pattern = compile(r'(([0-9]*\.)?[0-9]+)\s*\:\s+')
token_pattern = compile(
    r'(([^\s\"\(\)]|(\(([^\(\)]|(\"([^\"]|\\")+\"))+\))|(\"([^\"]|\\")*\"))+)\s*')


class Token:

    __slots__ = 'identifier', 'terminal', 'optional', 'probability'

    def __init__(self, string):
        self.terminal = False
        self.optional = False
        self.probability = 1
        if string.startswith('(') and string.endswith(')'):
            string = string[1:-1]
            matched = float_pattern.match(string)
            factor = 1
            if matched:
                string = string[matched.end():]
                factor = float(matched.group(1))
            self.__init__(string)
            self.optional = True
            self.probability *= factor
        elif string.startswith('"') and string.endswith('"'):
            self.identifier = string[1:-1].replace('\\"', '"')
            self.terminal = True
        else:
            self.identifier = string

    def __eq__(self, other):
        if not isinstance(other, Token):
            return False
        return self.identifier == other.identifier and \
            self.terminal == other.terminal

    def __hash__(self):
        return hash((self.identifier, self.terminal))

    def __str__(self):
        string = self.identifier.replace('"', '\\"')
        if self.terminal:
            string = '"{}"'.format(string)
        if self.optional:
            if self.probability == 1:
                string = '({})'.format(string)
            else:
                string = '({}: {})'.format(self.probability, string)
        return string


class Rule:

    __slots__ = 'lhs', 'rhs', 'weight'

    def __init__(self, string):
        matched = float_pattern.match(string)
        if matched:
            string = string[matched.end():]
            self.weight = float(matched.group(1))
        else:
            self.weight = 1
        matched = token_pattern.match(string)
        string = string[matched.end():]
        self.lhs = Token(matched.group(1))
        string = string[match(r'->\s*', string).end():]
        self.rhs = []
        while True:
            matched = token_pattern.match(string)
            if not matched:
                break
            string = string[matched.end():]
            self.rhs.append(Token(matched.group(1)))

    def __eq__(self, other):
        if not isinstance(other, Rule):
            return False
        return self.lhs, tuple(self.rhs), self.weight == \
            other.lhs, tuple(other.rhs), other.weight

    def __hash__(self):
        return hash((self.lhs, tuple(self.rhs), self.weight))

    def __str__(self):
        elements = []
        if self.weight != 1:
            elements.append(str(self.weight) + ':')
        elements += [str(self.lhs), '->']
        for element in self.rhs:
            elements.append(str(element))
        return ' '.join(elements)


class CFG:

    __slots__ = 'rules', 'convergence'

    def __init__(self, string=None):
        self.rules = {}
        self.convergence = 1
        if string:
            self.load_lines(string)

    def __str__(self):
        elements = []
        if self.convergence != 1:
            elements.append('convergence = {}'.format(self.convergence))
        for rule_list in self.rules.values():
            for rule in rule_list:
                elements.append(str(rule))
        return '\n'.join(elements)

    def load_line(self, line):
        line = line.strip()
        if line == '' or line.startswith('//'):
            return
        matched = match(r'convergence\s*=\s*(([0-9]*\.)?[0-9]+)\s*', line)
        if matched:
            self.convergence = float(matched.group(1))
            return
        rule = Rule(line)
        if rule.lhs in self.rules:
            self.rules[rule.lhs].append(rule)
        else:
            self.rules[rule.lhs] = [rule]

    def load_lines(self, string):
        for line in string.split('\n'):
            try:
                self.load_line(line)
            except:
                print('Failed to parse line: {}'.format(line))

    def generate(self, start=Token('S'), max_length=-1):
        weight_dict = {}
        for rule_list in self.rules.values():
            for rule in rule_list:
                weight_dict[rule] = rule.weight
        stack = [start]
        terminals = []
        while len(stack) > 0:
            token = stack.pop()
            if token.optional and random() > token.probability:
                continue
            if token.terminal:
                terminals.append(token.identifier)
                continue
            try:
                rule_list = self.rules[token]
            except:
                raise Exception('Failed to find rule for: {}'.format(token))
            weights = [weight_dict[rule] for rule in rule_list]
            rule = choices(rule_list, weights, k=1)[0]
            weight_dict[rule] *= self.convergence
            stack += rule.rhs[::-1]
            if max_length > 0 and len(terminals) > max_length:
                raise Exception('Exceed max length: {}'.format(max_length))
        return terminals


demo_grammar = '''
convergence = 0.3

0.9: S -> Clause "."
0.1: S -> Clause "while" S

Clause -> NP VP

0.9: NP -> Det (0.6: Adj) N
0.1: NP -> NP "and" NP

VP -> V NP
VP -> V

Det -> "a"
Det -> "the"

Adj -> "smart"
Adj -> "tired"
Adj -> "brown"

N -> "student"
N -> "laptop"
N -> "car"

V -> "drives"
V -> "walks"
V -> "leaves"
'''


if __name__ == '__main__':
    cfg = CFG(demo_grammar)
    print(cfg)
    while True:
        input('=' * 80)
        try:
            tokens = cfg.generate(max_length=30)
        except Exception as exception:
            print(exception)
        else:
            tokens[0] = tokens[0].capitalize()
            print(' '.join(tokens[:-1]) + tokens[-1])
