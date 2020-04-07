## Introduction

对软件的期望：
* 能interactive
* 能customize词库

对我的期望：
* 提高C和Python编码能力
* 学习data structure知识

## Objectives

### Minimize Startup Time

Justify: shorter startup time == better user experience

Metric: shorter is better. < 1s is preferred.

### Control Memory Usage

Justify: some old laptops or virtual machines has very limited memory space (< 2GB). Big memory usage can cause series slowdown or even system failure.

Metric: less is better. < 100M is preferred.

### Large, Expandable Dictionary Capacity

Justify: if the user are not satisfied with the provided dictionary data (nor am I), they can customize it in the future.

Metric: larger capacity is better. Unlimited ideally.

## Detailed Framework

### Language

Use C for the mandatory part
* high efficiency to minimize startup time
* more precise memory control to minimize memory usage

Use Python for the optional part:
* More libraries and utilities (e.g. Poisson sampling)
* Efficiency is not that important (since it is not an interactive program).

### Data Structure

#### Dynamic Array (struct Array, struct Node)

为了减少错误和更高效的使用内存，我implement了一个简单的dynamic array。当内存不足时，它会申请当前大小2倍的内存；当内存多余时，它会把内存折半。这减少了realloc和malloc的次数，提高了性能；同时也不会占用过多内存造成浪费。

我对字符串也设计了struct String并采取了相同的内存管理方式。

#### struct Trie

Use Trie to store dictionary entries.

Each node represents a letter. It has one parent and many children (because only consider ASCII characters, number of children <= alphabet size = 128)

#### Levinshtein Automata

Solve Levinshtein distance by filling out a matrix.

Search down to child nodes ==> push a letter to the automata
Search up back to parent ==> pop a letter from the automata
Minimum distance > max tolerance ==> quit search

#### CFG language model

* Start from one token
* Each time replace 1 token with >= 1 tokens
* End with all terminals

#### CFG script language

* describe CFG with Python is too wordy, and hard to modify
* write the description of the grammar in a txt file
* parse the text with regex library in Python
* fill in the Python data structure
* Support optional token (with specified probability) and rule weight (the weight based on to choose rule being executed)

## Future Work/Conclusion

### Pros

1. 基本解决性能问题：哪怕是debug模式编译的程序，数十万大小的字典可以在0.5s左右加载完。

2. 因为添加了script模式，每次程序加载前可以预执行命令，增强了程序的扩展性和可定制性。

3. Mandatory part全部使用C标准库，没有compiler extension或third-party library。编译工具使用CMake替换GNU-Make。这些都保证了程序的跨平台性。理论上程序可以在Windows，Linux和MacOS运行。

### Cons

1. 内存大：C版本相对于Python版本内存没有显著减少（都在200-300M）。经过计算，如果给每个Node单独分配结构体理论上的内存需要的确有这么多。
解决方案：Node压缩，如果在Trie树中出现有单链结构（linked list），压缩为一个Node包含一个字符串。

2. 只支持ascii字符。原因：C语言标准库对unicode的IO支持不好。因为unicode每个字符不定长，导致Node的大小无法在编译前给出，而需要在运行时动态分配。很难给出unicode character和integer的唯一映射，需要构建专门的hash function。
解决方案：运用成熟的unicode库。结合前一个问题，每个node应该包含一个char sequence而不是单独的char，而char sequence的长度由运行时决定。

3. Story writer不能正确处理特殊语法现象。单复数，第三人称单数，主语宾语代词，时态等等。
解决方案：给每一个token增加parameter，只有当rule的LHS和RHS有相同的参数时（agreement），rule才可能被执行。