#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>

int main()
{
    std::ifstream file("file/data2.txt");
    if (!file.is_open())
    {
        std::cerr << "无法打开文件!" << std::endl;
        return 1;
    }
    std::unordered_map<std::string, int> counter;
    std::string word;
    while (file >> word)
    {
        counter[word]++;
    }

    std::vector<std::pair<int, std::string>> sortWord;

    for (const auto& [key, value] : counter)
    {
        sortWord.emplace_back(value, key);
    }
    std::sort(sortWord.begin(), sortWord.end(), std::greater<>());
    std::cout << "出现频率最高的10个单词为：" << std::endl;

    // 防止出现单词数量少于10个导致的未定义行为
    size_t wordNum = std::min(sortWord.size(), size_t{ 10 });
    for (size_t i = 0; i < wordNum;i++)
    {
        std::cout << i + 1 << '.' << sortWord[i].second
            << ", 出现次数:" << sortWord[i].first << std::endl;
    }

    // file.close(); // ifstream析构的时候会自动关闭文件
    return 0;
}