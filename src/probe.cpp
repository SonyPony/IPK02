/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#include "probe.h"
#include <sstream>
#include <regex>
#include <utility>
#include <string>

using std::string;

Probe::Probe(const unsigned char data[]) {
    memcpy(m_size.bytes,
           data + 0,
           sizeof(m_size));
    memcpy(m_index.bytes,
           data + sizeof(m_size.value),
           sizeof(m_index.value));
    memcpy(m_timestamp.bytes,
           data + sizeof(m_size.value) + sizeof(m_index.value),
           sizeof(m_timestamp.value));
}

Byte *Probe::serialize() const {
    auto data = new unsigned char[Probe::SIZE + m_size.value];
    memcpy(data + 0, m_size.bytes, sizeof(m_size));
    memcpy(data + sizeof(m_size), m_index.bytes, sizeof(m_index));
    memcpy(data + sizeof(m_size) + sizeof(m_index), m_timestamp.bytes, sizeof(m_timestamp));

    return data;
}

Probe::Probe(int32_t index, nanoseconds timestamp) {
    m_index.value = index;
    m_timestamp.value = timestamp;
}

string Probe::toString() const {
    string data{"Probe(index="};

    data.append(std::to_string(m_index.value));
    data.append(", size=");
    data.append(std::to_string(m_size.value));
    data.append(", timestamp=");
    data.append(std::to_string(m_timestamp.value.count()));
    data.append(")");
    return data;
}

Timestamp& Probe::timestamp() {
    return m_timestamp;
}

void Probe::setSize(int size) {
    m_size.value = size;
}

int Probe::size() const {
    return m_size.value;
}