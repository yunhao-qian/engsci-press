from grammar import CFG
from lexicon import create_write_lexicon
from writer import Writer


def create_espg():
    create_write_lexicon()
    espg = CFG()
    with open('espg_base.txt', 'r') as stream:
        lines = stream.read()
    espg.load_lines(lines)
    with open('espg_lexicon.txt', 'r') as stream:
        lines = stream.read()
    espg.load_lines(lines)
    return espg


def default_input(message, default_value, variable_name):
    try:
        value = int(input(message))
    except:
        value = default_value
        print('{} set to {}.'.format(variable_name, default_value))
    return value


if __name__ == '__main__':
    print('Creating ESPG...\n')
    espg = create_espg()
    while True:
        paragraphs_per_article = default_input(
            'Paragraphs per article: ', 3, 'paragraphs_per_article')
        sentences_per_paragraph = default_input(
            'Sentences per paragraph: ', 10, 'sentences_per_paragraph')
        tokens_per_sentence = default_input(
            'Tokens per sentence: ', 15, 'tokens_per_sentence')
        title = input('Title: ')
        spacing = default_input('Spacing: ', 1, 'spacing')
        indent = default_input('Indent: ', 4, 'indent')
        writer = Writer(espg, paragraphs_per_article,
                        sentences_per_paragraph, tokens_per_sentence)
        print('Composing...\n')
        article = writer.generate(title, spacing, indent)
        print(article)
        print()
        print('Word count: {}'.format(article.word_count()))
        print('=' * 80)
