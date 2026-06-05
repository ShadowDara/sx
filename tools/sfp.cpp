#include <iostream>
#include <sfplib/parser.hpp>

int main(int argc, char *argv[]) {
    	if (argc >= 2) {
        run_sam_file(argv[1]);
    	}
    else {
        std::cout << "No Argument provided!\n";
    }
    return 0;
}