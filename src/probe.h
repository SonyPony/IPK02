/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <chrono>


using namespace std;
using namespace std::chrono;


using Byte = unsigned char;

union IntBytes {
    Byte bytes[sizeof(int)];
    int value;
};

union Timestamp {
    Byte bytes[sizeof(nanoseconds)];
    nanoseconds value;
};

class Probe {
    private:
        IntBytes m_size;
        IntBytes m_index;
        Timestamp m_timestamp{.value=0ns};

    public:
        static const auto SIZE = 2 * sizeof(IntBytes) + sizeof(Timestamp);

        explicit Probe(const unsigned char data[]);
        explicit Probe() = default;
        explicit Probe(int32_t index, nanoseconds timestamp);

        Byte * serialize() const;
        string toString() const;

        Timestamp& timestamp();
        int size() const;

        void setSize(int size);
};

template<typename T>
T currentTime() {
    time_point<system_clock> now =
            system_clock::now();
    auto duration = now.time_since_epoch();
    return duration_cast<T>(duration);
}

#endif //PROTOCOL_H
