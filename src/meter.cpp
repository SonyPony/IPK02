/**
 * Project in IPK BIT VUT 2017-2018.
 * @author Son Hai Nguyen xnguye16
*/

#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <chrono>
#include "probe.h"
#include "meter.h"
#include "connection.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#define DEBUG(x) cerr << (x) << endl;

using namespace std;

void sendProbes(Connection& connection, unsigned int probesCount, uint16_t probesSize);

int meter(ArgumentsMeter args) {
    unsigned int probesCount = 500;

    Connection connection(std::string(args.hostname), args.port, Probe::SIZE);
    connection.initClient(100000);

    unsigned int receivedProbes = 0;
    vector<nanoseconds> timestamps;
    vector<nanoseconds> receivedTimes;
    double probesTimeSum = 0;
    int totalProbesCount = 0;

    connection.setReadCallback([&totalProbesCount, &connection, &receivedProbes, &receivedTimes, &timestamps, &probesCount, &probesTimeSum](ustring data, struct sockaddr* sender) {
        try {
            nanoseconds receiveTime = currentTime<nanoseconds>();

            auto m = new Probe{data.c_str()};

            timestamps.push_back(m->timestamp().value);
            receivedTimes.push_back(receiveTime);

            probesTimeSum += std::chrono::duration_cast<std::chrono::nanoseconds>(
                    receiveTime - m->timestamp().value).count() / 1'000'000.;
        } catch (std::runtime_error &e) { cerr << "fuck" << endl << flush; }

        receivedProbes++;
        totalProbesCount++;
        if (receivedProbes == probesCount)
            connection.stopRead();
    });

    seconds measureStartTime = currentTime<seconds>();
    auto nanosecondsSort = [](nanoseconds a, nanoseconds b) -> bool {
        return a.count() < b.count();
    };
    vector<double> speeds;

    do {
        sendProbes(connection, probesCount, args.probeSize);
        connection.startRead();

        if(timestamps.size() < 1) {
            cerr << "No response from reflector." << endl;
            return 1;
        }

        std::sort(timestamps.begin(), timestamps.end(), nanosecondsSort);
        std::sort(receivedTimes.begin(), receivedTimes.end(), nanosecondsSort);

        double reflectedRatio = (double) receivedProbes / probesCount;

        speeds.push_back(((8 * (args.probeSize + 28) * receivedProbes) / 1000'000.) /
                         ((double)(receivedTimes.at(receivedTimes.size() - 1).count() - timestamps.at(0).count()) / 1'000'000'000));

        if(receivedProbes == 0)
            break;

        if(reflectedRatio < 0.9)
            probesCount /= 1.2;
        else if(reflectedRatio > 0.7)
            probesCount *= 1.2;
        receivedProbes = 0;

        timestamps.clear();
    } while(currentTime<seconds>().count() < measureStartTime.count() + args.measureTime);

    double avgSpeed = 0;
    for(double value: speeds) {
        avgSpeed += value;
    }
    avgSpeed /= speeds.size();

    double dispersion = 0;
    for(double value: speeds) {
        dispersion += pow(value - avgSpeed, 2);
    }
    dispersion = sqrt(dispersion / speeds.size());


    cout << "Average speed: " << avgSpeed << " Mbit/s" << endl
         << "Maximum speed: " << *std::max_element(speeds.begin(), speeds.end()) << " Mbit/s" << endl
         << "Minimum speed: " << *std::min_element(speeds.begin(), speeds.end()) << " Mbit/s" << endl
         << "Dispersion: " << dispersion << " Mbit/s" << endl
         << "Average RTT " << probesTimeSum / totalProbesCount << " ms" <<endl <<flush;

    connection.closeSocket();

    return 0;
}

void sendProbes(Connection& connection, unsigned int probesCount, uint16_t probesSize) {
    for (int i = 0; i < probesCount; i++) {
        Probe probe{1, currentTime<nanoseconds>()};
        probe.setSize(probesSize);

        if (0 > connection.sendData(probe.serialize(), Probe::SIZE + probe.size())) {
            DEBUG("Error send");
            exit(EXIT_FAILURE);
        }
    }
}
