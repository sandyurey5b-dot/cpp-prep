# WordCounter：单词频率统计

> week1 练习 · 2026-10-27

## 任务

读入一个文本文件，用 `std::unordered_map` 统计每个单词出现的次数，输出出现次数最多的 10 个单词。项目用 CMake 构建。

## 构建与运行

```bash
cmake -S . -B build
cmake --build build
cd build && ./WordCounter
```

程序读取的是相对路径 `file/data2.txt`（要换数据，改 `main.cpp` 里的路径即可）。CMake 在 configure 阶段会把 `file/` 目录复制到 `build/` 下，所以需要在 `build/` 目录里运行。

示例输出（`data2.txt`，只有 5 个不同的词，正好覆盖了"不足 10 个"的情况）：

```
出现频率最高的10个单词为：
1.apple, 出现次数:15
2.banana, 出现次数:14
3.orange, 出现次数:6
4.grape, 出现次数:5
5.kiwi, 出现次数:2
```

`data1.txt` 的结果：the(180)、and(160)、apple(140)……world(60)。

## 实现思路

1. 用 `std::ifstream` 打开文件，`file >> word` 按空白切分，逐词读取。
2. 用 `std::unordered_map<std::string, int>` 计数。
3. `unordered_map` 是无序的，没法直接取前 10。所以把每个元素以 `(次数, 单词)` 的形式放进 `std::vector<std::pair<int, std::string>>`。
4. 用 `std::sort` 配合 `std::greater<>` 降序排序，用 `std::min(sortWord.size(), size_t{10})` 算出要输出的个数，一个循环输出。

## 知识点

**文件读取**
- `operator>>` 读 `std::string` 时以空白为分隔符，`while (file >> word)` 在读到 EOF 或出错时结束。
- `std::ifstream` 析构时会自动关闭文件（RAII），不需要手动调用 `close()`。

**unordered_map**
- 访问不存在的 key 时，`operator[]` 会插入一个**值初始化**的 value（`int` 即 0），所以计数直接写 `counter[word]++` 就行。
- 先 `contains()` 再 `operator[]` 会查找两次，没有必要。

**排序**
- `std::pair` 的比较是字典序：先比 `first`，相等再比 `second`。把次数放在 `first`，就能直接用默认比较，不用自己写比较函数。
- `std::greater<T>` 的 `T` 是**被比较的元素类型**。最初写成了 `std::greater<int>`，而元素类型是 `pair<int, string>`，结果编译报错。
- `std::greater<>`（C++14 起）是透明比较器，会自动推导参数类型。

**其他**
- 结构化绑定 `for (const auto& [key, value] : counter)` 遍历 map，不产生拷贝。
- `emplace_back(value, key)` 原地构造 pair。
- 边界情况：不同单词不足 10 个时，写死循环 10 次会越界访问，属于未定义行为。改用 `std::min` 先算出上界。
- `std::min` 是模板 `min(const T&, const T&)`，两个参数必须推导成同一类型。`std::min(v.size(), 10)` 里一个是 `size_t`、一个是 `int`，推导冲突，编译不过，所以写成 `size_t{10}`。

**有符号 / 无符号**
- `size()` 返回无符号的 `size_t`。赋给 `int` 是窄化转换（`-Wconversion` 会报），拿 `int` 和 `size_t` 比较会触发 `-Wsign-compare`（包含在 `-Wall` 里）。
- 只开 `-Wconversion` 时看不到 `-Wsign-compare`，所以警告选项要开全。现在 CMake 里用 `target_compile_options(WordCounter PRIVATE -Wall -Wextra -Wpedantic -Wconversion)`，上界和循环变量统一用 `size_t`，编译零警告。
- 相对路径是相对于**进程的当前工作目录**解析的，不是相对于可执行文件所在目录。`file(COPY ...)` 只在 configure 时执行，数据文件修改后需要重新运行 cmake。

## 已知局限与可改进点

- **标点和大小写没有处理**：`Apple`、`apple`、`apple,` 会被算成三个不同的词。
- **次数相同时按字母倒序输出**：这是 `(次数, 单词)` 整体降序排序的副作用。如果要按字母正序，需要自定义比较器。
- **只要 Top 10 却对全部元素排了序**，复杂度 O(n log n)。可以改用 `std::partial_sort`（O(n log k)）、`std::nth_element`，或者维护一个大小为 k 的小顶堆。
- **文件路径写死在代码里**，后续可以改成从配置文件读取。
## 自测问题

- `counter[word]` 和 `counter.at(word)` 遇到不存在的 key 时，行为有什么不同？
- `std::greater<>` 和 `std::greater<int>` 有什么区别？
- `partial_sort`、`nth_element` 和堆三种做法，分别适合什么场景？
- 文件大到内存放不下时，该怎么统计？
