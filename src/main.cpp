/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#include <iostream>
#include "reflect.h"
#include "meter.h"
#include "argumentparser.h"

int main(int argc, const char *argv[]) {

    ArgumentParser parser(argc, argv);
    ArgumentParser::TypeMeasurer type = parser.parseType();


    if (type == ArgumentParser::TypeMeasurer::Reflector)
        return reflect(parser.parseServerArguments());
    else if (type == ArgumentParser::TypeMeasurer::Meter)
        return meter(parser.parseMeterArguments());

    std::cerr << "Unknown run type." << std::endl;
    exit(EXIT_FAILURE);
}