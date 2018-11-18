/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H


#include <cstdint>
#include <string>

struct ArgumentsServer {
    uint16_t port;
};

struct ArgumentsMeter {
    std::string hostname;
    uint16_t port;
    uint16_t probeSize;
    uint16_t measureTime;
};

class ArgumentParser {
    public:
        enum class TypeMeasurer {
                Reflector,
                Meter,
                None
        };

    private:
        int m_argc;
        const char** m_argv;
        TypeMeasurer m_type;

        void exitWithError(const char* msg) const;

    public:
        ArgumentParser(int argc, const char** argv);

        TypeMeasurer parseType();
        ArgumentsServer parseServerArguments() const;
        ArgumentsMeter parseMeterArguments() const;
};


#endif //ARGUMENTPARSER_H
