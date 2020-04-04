from collections import defaultdict


class DictEntry:

    __slots__ = 'headword', 'word_class'

    def __init__(self, headword, word_class):
        self.headword = headword
        self.word_class = word_class

    @staticmethod
    def from_line(line):
        if line.startswith('"') and line.endswith('"'):
            line = line[1:-1]
        left_index = line.index('(')
        if left_index < 0:
            raise Exception('missing word class', line)
        right_index = -1
        depth = 1
        for i in range(left_index + 1, len(line)):
            if line[i] == '(':
                depth += 1
            elif line[i] == ')':
                depth -= 1
                if depth == 0:
                    right_index = i
                    break
        if right_index < 0:
            raise Exception('mismatched brackets', line)
        headword = line[:left_index].strip()
        if headword == '':
            raise Exception('empty headword', line)
        word_class = line[left_index + 1:right_index].strip()
        return DictEntry(headword, word_class)


pos_tag_to_word_classes = {
    'Proper-Noun-Sg': set(),
    'Proper-Noun-Pl': set(),
    'Noun-Sg': {
        'n. & v',
        'n.& v.',
        'n & v.',
        'n. & v. t.',
        'n.',
        'n. sing & pl.',
        'a & n.',
        'n. & v. i.',
        'n. /',
        'n. / interj.',
        'n. & v.',
        'sing. or pl.',
        'n.sing & pl.',
        'n',
        'n., a., & v.',
        'n. & a.',
        'sing. & pl.',
        'n .',
        'v. t. & n.',
        'n. sing. & pl.',
        'a., n., & adv.',
        'n. & adv.',
        'n. / v. t. & i.',
        'n.sing. & pl.',
        'n. .',
        'v.& n.',
        'n. & interj.',
        'adv. & n.',
        'n. Chem.',
        'v. i. & n.',
        'n.',
        'sing.',
        'N.',
        'n./',
        'adv., & n.',
        'a. / n.',
        'v. & n.',
        'a., adv., & n.',
        'n..',
        'n. sing. & pl',
        'interj. & n.',
        'n. sing.',
        'n. & i.',
        'imperative sing.',
        'syntactically sing.'
    },
    'Noun-Pl': {
        'n. pl.',
        'n. sing & pl.',
        'n.pl.',
        'sing. or pl.',
        'n.sing & pl.',
        'sing. & pl.',
        'n. pl',
        'n. sing. & pl.',
        'n.sing. & pl.',
        'n pl.',
        'n., sing. & pl.',
        'n. collect. & pl.',
        'n. sing. & pl',
        'n. pl.',
        'sing. / pl.'
    },
    'Gerund-V': {
        'p. pr. & v. n.',
        'p. pr. &, vb. n.',
        'imp. & p. p. Fenced (/); p. pr. & vb. n.',
        'imp. & p. p. & vb. n.',
        'p, pr. & vb. n.',
        'p. pr. a. & vb. n.',
        'p. pr. vb. n.',
        'imp. & p. pr. & vb. n.',
        'pr.p. & vb. n.',
        'p. pr. / vb. n.',
        'p]. pr. & vb. n.',
        'p. pr.& vb. n.',
        'p. pr. &vb. n.',
        'p. pr. & vb/ n.',
        'P. pr. & vb. n.',
        'p. pr. & vvb. n.',
        'p. a. & vb. n.',
        'p. pr. &. vb. n.',
        'p. pr. & pr. & vb. n.',
        'vb. n.',
        'p. p. & vb. n.',
        'p pr. & vb. n.',
        'imp. & p. p. Adored (/); p. pr. & vb. n.',
        'p. pr & vb. n.'
    },
    'Verb-I-Sg': {
        '3d sing.pr.',
        'subj. 3d pers. sing.',
        '3d sing.',
        '3d pers. sing. pres.',
        '3d sing. pr.',
        'pres. indic. sing., 1st & 3d pers.',
        'Sing. pres. ind.',
        '3d sing.',
        'pres. sing.'
    },
    'Verb-T-Sg': {
        '3d sing.pr.',
        'subj. 3d pers. sing.',
        '3d sing.',
        '3d pers. sing. pres.',
        '3d sing. pr.',
        'pres. indic. sing., 1st & 3d pers.',
        'Sing. pres. ind.',
        '3d sing.',
        'pres. sing.'
    },
    'Verb-I-Pl': {
        'v. t. / i.',
        'v. i.,',
        'n. & v. i.',
        'v. i. & i.',
        'v. i.',
        'v.t & i.',
        'v.i',
        'v. t. / v. i.',
        'v.i.',
        'v. t.& i.',
        'n. / v. t. & i.',
        'v. i.',
        'v. t. & v. i.',
        'v. i. & n.',
        'v. i. & auxiliary.',
        'v. t. & i.',
        'v. i. & t.',
        'v. i. / auxiliary'
    },
    'Verb-T-Pl': {
        'v. t. / i.',
        'a. & v. t.',
        'v. t. &',
        'n. & v. t.',
        'v. t..',
        'v. t. v. t.',
        'v.t & i.',
        'v. t. / v. i.',
        'v.t',
        'v. t. & n.',
        'v. t.& i.',
        'n. / v. t. & i.',
        'v. t. & v. i.',
        'v./t.',
        'v. t.',
        'v. t. / auxiliary',
        'v. t.',
        'v. i. & t.',
        'v.t.'
    },
    'Preposition': {
        'prep., adv., & conj.',
        'prep., adv., conj. & n.',
        'adv. & prep.',
        'prep. & conj., but properly a participle',
        'prep., adv. & a.',
        'prep., adv. & conj.',
        'prep. & adv.',
        'adv., prep., & conj.',
        'prep.',
        'adv. or prep.',
        'prep. & conj.',
        'conj. & prep.'
    },
    'Adj': {
        'adj.',
        'pron. / adj.',
        'a.',
        'p. p. / a.',
        'a. & v. t.',
        'adv. & a.',
        'p. p & a.',
        'p. p. & a.',
        'a. / a. pron.',
        'P. p. & a.',
        'pron. & a.',
        'a & n.',
        'a/',
        'adv. / a.',
        'a. & a. pron.',
        'a & p. p.',
        'p. & a.',
        'prep., adv. & a.',
        'a. .',
        'a. superl.',
        'v. & a.',
        'a. & adv.',
        'n., a., & v.',
        'a. a.',
        'pron., a., conj., & adv.',
        'n. & a.',
        'p. pr. a. & vb. n.',
        'a. & v.',
        'a., n., & adv.',
        'a. Vigorously',
        'a. & n.',
        'a.',
        'a. / adv.',
        'a & adv.',
        'a. Vibrating',
        'a. or pron.',
        'a. / pron.',
        'imp., p. p., & a.',
        'a',
        'p. p. & a',
        'a. / n.',
        'pron., a., & adv.',
        'a., adv., & n.',
        'a. & p. p.',
        'a. & pron.'
    },
    'Adv': {
        'prep., adv., & conj.',
        'prep., adv., conj. & n.',
        'adv. & a.',
        'adv. In combination or cooperation',
        'adv. / interj.',
        'interrog. adv.',
        'adv. & prep.',
        'adv. In a vanishing manner',
        'adv. / a.',
        'prep., adv. & a.',
        'a. & adv.',
        'pron., a., conj., & adv.',
        'prep., adv. & conj.',
        'conj. / adv.',
        'adv.',
        'prep. & adv.',
        'interj., adv., or a.',
        'a., n., & adv.',
        'interj., adv., & n.',
        'n. & adv.',
        'a. / adv.',
        'adv., prep., & conj.',
        'adv. & n.',
        'a & adv.',
        'adv. or prep.',
        'adv., & n.',
        'pron., a., & adv.',
        'a., adv., & n.',
        'interj. & adv.',
        'adv. / conj.',
        'adv. & conj.'
    }
}


def create_lexicon(lines):
    lexicon = {}
    for tag in pos_tag_to_word_classes:
        lexicon[tag] = set()
    for line in lines:
        line = line.strip()
        if line == '':
            continue
        try:
            entry = DictEntry.from_line(line)
        except:
            print('Failed to parse line: {}'.format(line))
            continue
        for tag in pos_tag_to_word_classes:
            word_class_set = pos_tag_to_word_classes[tag]
            if entry.word_class in word_class_set:
                lexicon[tag].add(entry.headword.lower().replace('"', '\\"'))
    return lexicon


def write_lexicon(lexicon, stream):
    for tag in pos_tag_to_word_classes:
        for terminal in lexicon[tag]:
            stream.write('{} -> "{}"\n'.format(tag, terminal))


if __name__ == '__main__':
    try:
        output = open('espg_lexicon.txt', 'w')
    except:
        print('Failed to open file: {}'.format('espg_lexicon.txt'))
        exit()
    for i in range(ord('A'), ord('Z') + 1):
        file_name = ('../../Dictionary-in-csv/{}.csv'.format(chr(i)))
        try:
            with open(file_name, 'r') as stream:
                lines = open(file_name, 'r').readlines()
        except:
            print('Failed to open file: {}'.format(file_name))
        else:
            lexicon = create_lexicon(lines)
            write_lexicon(lexicon, output)
    output.close()
