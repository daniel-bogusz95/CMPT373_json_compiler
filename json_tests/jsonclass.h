#pragma once
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

namespace Json 
{
  class parser 
  {
    private: 
        json data;

    public:
  	    parser();
        void write_file(json &j);
        void read_file(json &j);
  };
}
