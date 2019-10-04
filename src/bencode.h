#ifndef HYDRAFS_BENCODE_HEADER
#define HYDRAFS_BENCODE_HEADER

#include <ostream>
#include <string>
#include <vector>


namespace hydrafs{
namespace bencode{

struct Value{
    Value() = default;

    Value(std::string bytes):
        _string(std::move(bytes)), dtype(STR)
    {}

    Value(int integer):
        _integer(integer), dtype(INT)
    {}

    Value(std::vector<std::pair<std::string, Value>>  dict):
        _dict(std::move(dict)), dtype(DICT)
    {}

    Value(std::vector<Value>  list):
        _list(std::move(list)), dtype(LIST)
    {}

    std::string const& string() const {
        return _string;
    }

    int integer() const {
        return _integer;
    }

    std::vector<Value> const& list() const {
        return _list;
    }

    std::vector<std::pair<std::string, Value>> const& dict() const {
        return _dict;
    }

    bool is_integer() const { return dtype == INT; }
    bool is_string() const { return dtype == STR; }
    bool is_list() const { return dtype == LIST; }
    bool is_dict() const { return dtype == DICT; }

    std::ostream& print(std::ostream& out) const;

private:
    std::string _string;
    int _integer = 0;
    std::vector<Value> _list;
    std::vector<std::pair<std::string, Value>> _dict;

public:
    const enum {
        INT,
        STR,
        LIST,
        DICT,
        NONE
    } dtype = NONE;
};

Value parse_bencode(FILE* file, int depth = 0);

std::ostream& pprint(std::ostream& out, Value const& val, std::size_t depth = 0);

} // namespace bencode
} // namespace hydrafs

#endif
