#include "bencode.h"

#include <cstdio>
#include <sstream>


// #define DEBUG(out) std::cout << out << std::endl
#define DEBUG(X)

namespace hydrafs{
namespace bencode{

std::ostream& Value::print(std::ostream& out) const {
    switch(dtype){
    case NONE: return out;
    case INT: return out << integer();
    case STR: return out << string();
    case LIST:{
        out << '[';
        for (auto& v: list()){
            v.print(out) << ", ";
        }
        out << ']';
        return out;
    }
    case DICT:{
        out << '{';
        for (auto& kv: dict()){
            out << '"' << kv.first << "\": ";
            kv.second.print(out) << ", ";
        }
        out << '}';
        return out;
    }
    }
    return out;
}

// leaf
int parse_int(FILE* file, int depth){
    DEBUG(std::string(depth , ' ') << "Integer");

    int integer_value;
    std::string value;

    int c = fgetc(file);
    while (c != 'e'){
        value.push_back(char(c));
        c = fgetc(file);
    }

    std::stringstream(value) >> integer_value;

    DEBUG(std::string(depth , ' ') << "Integer: " << integer_value);
    return integer_value;
}

std::string parse_bytes(FILE* file, int depth){
    DEBUG(std::string(depth , ' ') << "Bytes");

    int c = fgetc(file);
    std::string value;
    std::size_t size = 0;

    // parse size
    while (c != ':'){
        value.push_back(char(c));
        c = fgetc(file);
    }
    std::stringstream(value) >> size;
    value = std::string(size, ' ');

    for(std::size_t i = 0; i < size; ++i){
        value[i] = char(fgetc(file));
    }

    DEBUG(std::string(depth , ' ') << "Bytes: " << value << " size: " << size);
    return value;
}

std::vector<Value> parse_list(FILE* file, int depth){
    DEBUG(std::string(depth , ' ') << "list");

    int c = fgetc(file);
    std::vector<Value> list;

    while (c != 'e'){
        ungetc(c, file);
        list.push_back(parse_bencode(file, depth + 1));
        c = fgetc(file);
    }

    DEBUG(std::string(depth , ' ') << "List: " << list.size());
    return list;
}

std::vector<std::pair<std::string, Value>> parse_dict(FILE* file, int depth){
    DEBUG(std::string(depth , ' ') << "dict");

    int c = fgetc(file);
    using Dict = std::vector<std::pair<std::string, Value>>;

    Dict dict;
    while (c != 'e'){
        ungetc(c, file);
        std::string key = parse_bytes(file, depth + 1);
        //dict[key] = parse_bencode(file, depth + 1);
        dict.emplace_back(key, parse_bencode(file, depth + 1));
        c = fgetc(file);
    }

    DEBUG(std::string(depth , ' ') << "dict: " << dict.size());
    return dict;
}

Value parse_bencode(FILE* file, int depth){
    DEBUG(std::string(depth , ' ') << "bencode");

    int c = fgetc(file);

    switch(c){
    case 'i':
        return Value(parse_int(file, depth + 1));

    case 'l':
        return Value(parse_list(file, depth + 1));

    case 'd':
        return Value(parse_dict(file, depth + 1));

    default:
        ungetc(c, file);
        return Value(parse_bytes(file, depth + 1));
    }
}

std::ostream& pprint(std::ostream& out, Value const& val, std::size_t depth) {
    switch(val.dtype){
    case Value::NONE: return out;
    case Value::INT: return out << val.integer();
    case Value::STR: return out << val.string();
    case Value::LIST:{
        out << "[\n";
        for (auto& v: val.list()){
            out << std::string(depth + 1, ' ');
            pprint(out, v, depth + 1) << ",\n";
        }
        out << std::string(depth, ' ') << ']';
        return out;
    }
    case Value::DICT:{
        out << "{\n";
        for (auto& kv: val.dict()){
            out << std::string(depth + 1, ' ') << '"' << kv.first << "\": ";

            if (kv.first != "pieces"){
                pprint(out, kv.second, depth + 1) << ",\n";
            } else {
                out << "<pieces:" << kv.second.string().size() / 20 << ">,\n";
            }
        }
        out <<  std::string(depth, ' ') << '}';
        return out;
    }
    }
    return out;
}

} // namespace bencode
} // namespace hydrafs
