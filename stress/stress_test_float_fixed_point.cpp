#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include "../main.h"



int main(int argc, char* argv[]) {

    assert(argc == 2);

    std::string command = argv[1];

    std::string operation;
    std::string input_file_name, output_file_name;

    if (command == "+") operation = "plus";
    else if (command == "-") operation = "minus";
    else if (command == "*") operation = "mul";
    else if (command == "/") operation = "div";
    else assert(false);
    input_file_name = "fixed_" + operation + "_test";
    output_file_name = "fixed_" + operation + "_out";

    std::string input_dir_name = "/Users/apple/Desktop/SKKV/math-cs-comp-arch-24-fixed-floating-gbu1at/src/stress/float_fixed_point/input";
    std::string output_dir_name = "/Users/apple/Desktop/SKKV/math-cs-comp-arch-24-fixed-floating-gbu1at/src/stress/float_fixed_point/output";

    std::ifstream in(input_dir_name + "/" + input_file_name);

    std::cout << input_dir_name + "/" + input_file_name << "\n";

    std::ifstream out(output_dir_name + "/" + output_file_name);

    assert(in.is_open());
    assert(out.is_open());

    int t; in >> t;
    int N = t;

    int errors = 0;

    std::vector<int> tests_errors;

    while (t--) {

        std::string s1, s2, s3, s4, s5, s6;
        
        in >> s1 >> s2 >> s3 >> s4 >> s5 >> s6;

        std::string concat = s1 + "." + s2;

        char* argv[] = {
            const_cast<char*>("programm"), 
            const_cast<char*>(concat.c_str()), 
            const_cast<char*>(s3.c_str()), 
            const_cast<char*>(s4.c_str()), 
            const_cast<char*>(s5.c_str()), 
            const_cast<char*>(s6.c_str())
        };

        auto my_result = command_float_fixed_point(6, argv);

        std::string correct_result; out >> correct_result;

        if (my_result != correct_result) {
            std::cout << "TEST " << N - t << " is wrong!\n";
            std::cout << "\n";
            std::cout << "My result: " <<  my_result << "\tCorrect result: " << correct_result << "\n"; 
            std::cout << "Expression:\t" << s1 << " " << s2 << " " << s3 << " " << s4 << " " << s5 << " " << s6 << "\n";
            std::cout << "\n";
            errors++;
            // break;
            // tests_errors.push_back(N - t);
        } else {
            std::cout << "TEST " << N - t << " is passed!\n";
        }
    }
    std::cout << "\n\n";
    std::cout << "Persent errors:\t" << errors << "/" << N << "\n";
    std::cout << "\n";
    for (int x : tests_errors)
        std::cout << x << " ";
    std::cout << "\n\n";

    in.close();
    out.close();


    return 0;
}