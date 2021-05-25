"""
    Name: Daniel Schlatter
    Date: 6/28/19
    Class: CSCI 3415
    Description: Program to take a txt file of grammar rules, parse it,
    then derive user-entered sentences based on those rules.
   """
import os
import re
import sys


def isnonterminal(symbol):
    """Return true if symbol is a string,
       and is a nonterminal symbol.
    """
    return isinstance(symbol, str) and re.match(r"^<.*>$", symbol) is not None


def read_grammar(filepath):
    """ Parses file into grammar and returns grammar."""
    grammar = []
    current_lhs = None

    def make_rule(lhs, rhs):
        if not isnonterminal(lhs):
            raise Exception("LHS {} is not a nonterminal".format(lhs))
        if not rhs:
            raise Exception("Empty RHS")
        return (lhs, rhs)

    def parse_rhs(lexemes):
        rules = []
        rhs = []
        for lex in lexemes:
            if lex == "|":
                rules.append(make_rule(current_lhs, rhs))
                rhs = []
            else:
                rhs.append(lex)
        rules.append(make_rule(current_lhs, rhs))
        return rules

    with open(filepath) as grammar_file:
        for line in grammar_file:
            lexemes = line.split()
            if not lexemes:
                pass
            elif len(lexemes) == 1:
                raise Exception("Illegal rule {}".format(line))
            elif isnonterminal(lexemes[0]) and lexemes[1] == "->":
                current_lhs = lexemes[0]
                grammar.extend(parse_rhs(lexemes[2:]))
            elif lexemes[0] == "|":
                grammar.extend(parse_rhs(lexemes[1:]))
            else:
                raise Exception("Illegal rule {}".format(line))

    return grammar


def print_grammar(grammar):
    """ Prints the parsed grammar rules to the console."""
    for rule in grammar:
        print("{} -> {}".format(rule[0], " ".join(rule[1])))


def start_symbol(grammar):
    """ Returns first nonterminal of the grammar file."""
    return grammar[0][0]


def derive(grammar, form, sentence):
    """Derives the entered sentence using the previously established grammar rules."""
    def applicable_rules(grammar, nonterminal):
        """ Return a list of grammar rules for nonterminal. """
        return list(filter(lambda rule: rule[0] == nonterminal, grammar))
    def match(form, sentence):
        for i, lexeme in enumerate(form):
            if i == len(sentence):
                return -1
            if isnonterminal(lexeme):
                return i
            if lexeme != sentence[i]:
                return -1
        return len(sentence) if len(sentence) == len(form) else -1
    def subst(rule, form, match):
        return form[:match] +rule[1] +form[match+1:]
    match_res = match(form, sentence)
    if match_res == -1:
        return None
    if match_res == len(sentence):
        return []
    for rule in applicable_rules(grammar, form[match_res]):
        next_form = subst(rule, form, match_res)
        if len(next_form) <= len(sentence):
            derivation = derive(grammar, next_form, sentence)
            if derivation is not None:
                return [next_form] + derivation
    return None


def print_derivation(derivation, start):
    """ Prints derivation to the console."""
    if derivation is None:
        print("No derivation found.")
    else:
        for i in enumerate(derivation, 0):
            print("{}{}: {} -> {}".format(" " if i[0] < 10 else "", i[0],
                                          start if i[0] == 0 else len(start)*' ',
                                          " ".join(i[1])))


def main():
    """ Driver for program. Makes function calls. """
    filepath = sys.argv[1]

    if not os.path.isfile(filepath):
        raise Exception("File path {} does not exist.".format(filepath))

    print("Reading grammar from {}".format(filepath))
    grammar = read_grammar(filepath)
    print_grammar(grammar)
    while grammar:
        print("---")
        print("Enter a sentence:")
        sentence = input()
        print("Sentence:")
        print(sentence)
        print("Derivation:")
        sentence = sentence.split()
        derivation = derive(grammar, [start_symbol(grammar)], sentence)
        print_derivation(derivation, start_symbol(grammar))

if __name__ == "__main__":
    main()
