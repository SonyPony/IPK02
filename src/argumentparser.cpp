/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#include "argumentparser.h"
#include <iostream>
#include <string>
#include <getopt.h>

ArgumentParser::ArgumentParser(int argc, const char **argv) {
    m_argc = argc;
    m_argv = argv;
    m_type = ArgumentParser::TypeMeasurer::None;
}

ArgumentParser::TypeMeasurer ArgumentParser::parseType() {
    if(m_argc <= 1)
        this->exitWithError("Bad arguments");

    if(std::string(m_argv[1]) == "reflect")
        m_type = ArgumentParser::TypeMeasurer::Reflector;
    else if(std::string(m_argv[1]) == "meter")
        m_type = ArgumentParser::TypeMeasurer::Meter;
    else
        this->exitWithError("Bad arguments");
    return m_type;
}

void ArgumentParser::exitWithError(const char *msg) const {
    using namespace std;
    cerr << msg << endl << flush;
    exit(EXIT_FAILURE);
}

ArgumentsServer ArgumentParser::parseServerArguments() const {
    if(m_type != ArgumentParser::TypeMeasurer::Reflector)
        exitWithError("Need to call parse type first or bad type");

    if (m_argc != 4 || std::string(m_argv[2]) != "-p")
        this->exitWithError("Invalid arguments!");

    ArgumentsServer args{};
    args.port = static_cast<uint16_t>(strtol(m_argv[3], nullptr, 10));

    return args;
}

ArgumentsMeter ArgumentParser::parseMeterArguments() const {
    int c;
    bool hostFlag = false;
    bool portFlag = false;
    bool probeSizeFlag = false;
    bool measureTimeFlag = false;
    std::string host, port, probeSize, measureTime;

    while ((c = getopt(m_argc - 1, (char *const *)(m_argv + 1), ":h:p:s:t:")) != -1)
        switch (c) {
            case 'h':
                hostFlag = true;
                host = optarg;
                break;

            case 'p':
                portFlag = true;
                port = optarg;
                break;

            case 's':
                probeSizeFlag = true;
                probeSize = optarg;
                break;

            case 't':
                measureTimeFlag = true;
                measureTime = optarg;
                break;

            default:
                this->exitWithError("Wrong arguments");
        }

    ArgumentsMeter args{};
    args.port = static_cast<uint16_t>(strtol(port.c_str(), nullptr, 10));
    if (args.port < 0 || args.port > 65535)
        this->exitWithError("Invalid port");

    if (!hostFlag || !portFlag)
        this->exitWithError("Invalid args");

    if ((probeSizeFlag + measureTimeFlag) != 2)
        this->exitWithError("Erong arguments");

    args.hostname = host;
    args.measureTime = static_cast<uint16_t>(strtol(measureTime.c_str(), nullptr, 10));
    args.probeSize = static_cast<uint16_t>(strtol(probeSize.c_str(), nullptr, 10));

    return args;
}
