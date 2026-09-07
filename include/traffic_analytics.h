#ifndef TRAFFIC_ANALYTICS_H
#define TRAFFIC_ANALYTICS_H

#include "types.h"

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace DPI {

class TrafficAnalytics {
public:
    struct TrafficStat {
        uint64_t packets = 0;
        uint64_t bytes = 0;
    };

    // Record information about every processed packet
    void recordPacket(
        const FiveTuple& tuple,
        size_t packet_size,
        AppType app,
        const std::string& sni
    );

    // Print analytics report
    void printReport() const;

private:
    std::unordered_map<uint32_t, TrafficStat> source_ips_;
    std::unordered_map<uint32_t, TrafficStat> destination_ips_;

    std::unordered_map<uint8_t, TrafficStat> protocols_;

    std::unordered_map<AppType, TrafficStat> applications_;

    std::unordered_map<std::string, TrafficStat> domains_;

    mutable std::mutex mutex_;

    static std::string formatIP(uint32_t ip);
    static std::string formatBytes(uint64_t bytes);
};

} // namespace DPI

#endif