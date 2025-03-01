#include <cstddef>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) try {
    // 参数验证
    if (argc != 4) {
        throw std::runtime_error("参数错误\n用法: 程序名 <文件路径> <搜索字符串> <读取长度>");
    }

    // 解析参数
    const fs::path input_path(argv[1]);
    const std::string_view target_str(argv[2]);
    const int read_length = [&]{
        size_t pos;
        const int val = std::stoi(argv[3], &pos);
        if (pos != strlen(argv[3]) || val < 0)
            throw std::invalid_argument("无效长度值");
        return val;
    }();

    // 文件存在性检查
    if (!fs::exists(input_path)) {
        throw std::runtime_error("输入文件不存在");
    }

    // 读取文件内容
    std::ifstream input_file(input_path, std::ios::binary | std::ios::ate);
    const auto file_size = input_file.tellg();
    input_file.seekg(0);
    
    std::vector<char> file_data((size_t)file_size);
    if (!input_file.read(file_data.data(), file_size)) {
        throw std::runtime_error("文件读取失败");
    }

    // 搜索目标字符串
    const auto search_start = std::ranges::search(
        file_data, 
        std::string_view(target_str)
    );

    if (search_start.empty()) {
        throw std::runtime_error("未找到目标字符串");
    }

    // 验证读取范围
    const auto start_pos = std::distance(file_data.begin(), search_start.begin());
    if (start_pos + read_length > file_size) {
        throw std::runtime_error("读取长度超出文件范围");
    }

    // 构建输出路径
    auto output_path = input_path;
    output_path.replace_filename(
        input_path.stem().string() + "_span" + input_path.extension().string()
    );

    // 写入输出文件
    std::ofstream output_file(output_path, std::ios::binary);
    output_file.write(&file_data[(size_t)start_pos], read_length);
    
    if (!output_file) {
        throw std::runtime_error("文件写入失败");
    }

} catch (const std::exception& e) {
    std::cerr << "错误: " << e.what() << '\n';
    return EXIT_FAILURE;
}