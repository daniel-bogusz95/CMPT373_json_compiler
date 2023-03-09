#include "jsonclass.h"

Json::parser parser()
{
    std::ifstream file("test_read.json");
    Json::Value file_ptr;
    Json::Reader file_reader;

    file_reader.parse(file, file_ptr);

    auto Owner = file_ptr["Owner"];
    auto Game = file_ptr["Game"];
    auto Server = file_ptr["Server"];
}

void Json::parser::write_file(json &json_object)
{
    std::ofstream o("test_write.json");
    o << std::setw(4) << json_object << std::endl;
}

void Json::parser::read_file(json &j)
{

}