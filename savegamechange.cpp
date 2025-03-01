#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <ios>
#include <iostream>
#include <fstream>
#include <span>
#include <string_view>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;


struct party_data{

    uint32_t type;

    uint32_t count;

    uint32_t a;

    uint32_t b;
};


std::vector<char> readFile(const fs::path& path){
    // 读取文件内容
    std::ifstream input_file(path, std::ios::binary | std::ios::ate | std::ios::in);
    const auto file_size = input_file.tellg();
    input_file.seekg(0);
    
    std::vector<char> file_data((size_t)file_size);
    if (!input_file.read(file_data.data(), file_size)) {
        throw std::runtime_error("文件读取失败");
    }

    return file_data;
}


void writeFile(const fs::path& path, const std::vector<char>& file_data){
     // 写入输出文件
    std::ofstream output_file(path, std::ios::binary | std::ios::out);
    output_file.write(file_data.data(), (long long)file_data.size());
    
    if (!output_file) {
        throw std::runtime_error("文件写入失败");
    }
}


template<typename T>
void change_byte_data(char* data, size_t count, std::function<void(T&)> func){
    auto COPYBYTECOUNT = sizeof(T)*count;

    std::vector<T> buf{};
    buf.resize(count);

    std::memcpy(buf.data(), data, COPYBYTECOUNT);

   for (auto& item : buf) {
        func(item);
   }

   std::memcpy(data, buf.data(), COPYBYTECOUNT);

}

int main(int argc, char* argv[]) try {
    // 参数验证
    if (argc != 2) {
        throw std::runtime_error("参数错误\n用法: 程序名 <文件路径> <搜索字符串> <读取长度>");
    }
    const std::string_view target_str{"p_main_party"};
    // 解析参数
    const fs::path input_path(argv[1]);
   
    // 文件存在性检查
    if (!fs::exists(input_path)) {
        throw std::runtime_error("输入文件不存在");
    }

    auto file_data = readFile(input_path);

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
    
    const auto namelengthindex = (size_t)start_pos+target_str.size();

    const uint32_t* const namelength = (uint32_t*)&file_data[namelengthindex];

   
    //std::endian
    //std::byteswap
    //__builtin_bswap32 
    //_byteswap_ulong

    std::cout<< *namelength<<std::endl;

    
    const auto countindex = namelengthindex+4+*namelength+ 68;


    const uint32_t count = *((uint32_t*)&file_data[countindex]);
 
    std::cout << count<<std::endl;

    const auto  itemindex = countindex+4;

    change_byte_data<party_data>(&file_data[itemindex], count,[](party_data& item){

        if(item.type==0){return ;}

        std::cout  <<"type:"<< item.type<<" count:" << item.count<<std::endl;


        uint32_t incount;

        std::cin>>incount;

        item.count=incount;

        std::cout<<"change" <<"type:"<< item.type<<" count:" << item.count<<std::endl;

    });


    writeFile(input_path, file_data);




} catch (const std::exception& e) {
    std::cerr << "错误: " << e.what() << '\n';
    return EXIT_FAILURE;
}