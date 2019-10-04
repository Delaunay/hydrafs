#include <cstdio>
#include <iostream>

#include "bencode.h"

// #define DEBUG(out) std::cout << out << std::endl
#define DEBUG(X)


using namespace hydrafs;

int main(){

    FILE* file = fopen("/home/setepenre/work/hydrafs/examples/manjaro-kde-18.1.0-stable-x86_64.iso.torrent", "r");

    auto val = bencode::parse_bencode(file);
    pprint(std::cout, val);

    return 0;
}
