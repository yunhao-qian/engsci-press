from string import punctuation
from numpy.random import poisson


class Sentence:

    __slots__ = 'tokens'

    def __init__(self, tokens):
        self.tokens = tokens

    def __str__(self):
        elements = []
        for token in self.tokens:
            if token in punctuation and len(elements) > 0:
                elements[-1] = elements[-1] + token
            else:
                if len(elements) == 0:
                    elements.append(token.capitalize())
                else:
                    elements.append(token)
        return ' '.join(elements)

    def word_count(self):
        count = 0
        for token in self.tokens:
            if token not in punctuation:
                count += 1
        return count


class Paragraph:

    __slots__ = 'sentences', 'indent'

    def __init__(self, indent):
        self.sentences = []
        self.indent = indent

    def __str__(self):
        elements = []
        for sentence in self.sentences:
            elements.append(str(sentence))
        return ' ' * self.indent + ' '.join(elements)

    def add_sentence(self, tokens):
        self.sentences.append(Sentence(tokens))

    def word_count(self):
        count = 0
        for sentence in self.sentences:
            count += sentence.word_count()
        return count


class Article:

    __slots__ = 'paragraphs', 'title', 'spacing', 'indent'

    def __init__(self, title, spacing, indent):
        self.paragraphs = []
        self.title = title
        self.spacing = spacing
        self.indent = indent

    def __str__(self):
        elements = []
        if self.title:
            elements.append(self.title)
        for paragraph in self.paragraphs:
            if len(paragraph.sentences) == 0:
                continue
            elements.append(str(paragraph))
        return ('\n' * (self.spacing + 1)).join(elements)

    def add_sentence(self, tokens):
        if len(self.paragraphs) == 0:
            self.new_paragraph()
        self.paragraphs[-1].add_sentence(tokens)

    def new_paragraph(self, indent=None):
        if len(self.paragraphs) > 0 and \
                len(self.paragraphs[-1].sentences) == 0:
            self.paragraphs.pop()
        if indent == None:
            indent = self.indent
        self.paragraphs.append(Paragraph(indent))

    def word_count(self):
        count = 0
        for paragraph in self.paragraphs:
            count += paragraph.word_count()
        return count


class Writer:

    __slots__ = 'grammar', 'paragraphs_per_article',\
        'sentences_per_paragraph', 'tokens_per_sentence'

    def __init__(self, grammar, paragraphs_per_article=5,
                 sentences_per_paragraph=10, tokens_per_sentence=20):
        self.grammar = grammar
        self.paragraphs_per_article = paragraphs_per_article
        self.sentences_per_paragraph = sentences_per_paragraph
        self.tokens_per_sentence = tokens_per_sentence

    def generate(self, title=None, spacing=1, indent=4):
        article = Article(title, spacing, indent)
        for i in range(poisson(self.paragraphs_per_article)):
            for j in range(poisson(self.sentences_per_paragraph)):
                attempt_count = 0
                while True:
                    try:
                        max_length = poisson(self.tokens_per_sentence)
                        tokens = self.grammar.generate(max_length=max_length)
                    except:
                        attempt_count += 1
                        if attempt_count > 50:
                            raise Exception('Too many attempts')
                    else:
                        article.add_sentence(tokens)
                        break
            article.new_paragraph()
        return article


if __name__ == '__main__':
    from grammar import CFG, demo_grammar
    article = Writer(CFG(demo_grammar)).generate('Demo Article')
    print(article)
    print()
    print('Word count: {}'.format(article.word_count()))
