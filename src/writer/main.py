from grammar import CFG
from writer import Writer


def create_espg():
    espg = CFG()
    with open('espg_base.txt', 'r') as stream:
        lines = stream.read()
    espg.load_lines(lines)
    with open('espg_lexicon.txt', 'r') as stream:
        lines = stream.read()
    espg.load_lines(lines)
    return espg


if __name__ == '__main__':
    espg = create_espg()
    writer = Writer(espg, 3, 10, 15)
    while True:
        input('------------------------------------------------------------')
        article = writer.generate()
        print(article)
        print()
        print('Word count: {}'.format(article.word_count()))
