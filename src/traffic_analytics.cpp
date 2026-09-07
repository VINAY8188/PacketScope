#include "traffic_analytics.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

namespace DPI {

void TrafficAnalytics::recordPacket(
    const FiveTuple& tuple,
    size_t packet_size,
    AppType app,
    const std::string& sni
) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Source IP statistics
    source_ips_[tuple.src_ip].packets++;
    source_ips_[tuple.src_ip].bytes += packet_size;

    // Destination IP statistics
    destination_ips_[tuple.dst_ip].packets++;
    destination_ips_[tuple.dst_ip].bytes += packet_size;

    // Protocol statistics
    protocols_[tuple.protocol].packets++;
    protocols_[tuple.protocol].bytes += packet_size;

    // Application statistics
    applications_[app].packets++;
    applications_[app].bytes += packet_size;

    // Domain / SNI statistics
    if (!sni.empty()) {
        domains_[sni].packets++;
        domains_[sni].bytes += packet_size;
    }
}

std::string TrafficAnalytics::formatIP(uint32_t ip) {
    std::ostringstream ss;

    ss << ((ip >> 0) & 0xFF) << "."
       << ((ip >> 8) & 0xFF) << "."
       << ((ip >> 16) & 0xFF) << "."
       << ((ip >> 24) & 0xFF);

    return ss.str();
}

std::string TrafficAnalytics::formatBytes(uint64_t bytes) {
    std::ostringstream ss;

    if (bytes >= 1024 * 1024) {
        ss << std::fixed << std::setprecision(2)
           << (static_cast<double>(bytes) / (1024.0 * 1024.0))
           << " MB";
    }
    else if (bytes >= 1024) {
        ss << std::fixed << std::setprecision(2)
           << (static_cast<double>(bytes) / 1024.0)
           << " KB";
    }
    else {
        ss << bytes << " B";
    }

    return ss.str();
}

void TrafficAnalytics::printReport() const {

    std::lock_guard<std::mutex> lock(mutex_);

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "                 NETWORK TRAFFIC ANALYTICS\n";
    std::cout << "============================================================\n";

    // ---------------------------------------------------------
    // Top Source IPs
    // ---------------------------------------------------------

    std::vector<std::pair<uint32_t, TrafficStat>> sources(
        source_ips_.begin(),
        source_ips_.end()
    );

    std::sort(
        sources.begin(),
        sources.end(),
        [](const auto& a, const auto& b) {
            return a.second.bytes > b.second.bytes;
        }
    );

    std::cout << "\n[Top Source IPs]\n";

    int count = 0;

    for (const auto& [ip, stat] : sources) {

        std::cout << "  "
                  << (count + 1)
                  << ". "
                  << std::setw(15)
                  << formatIP(ip)
                  << " | packets: "
                  << std::setw(6)
                  << stat.packets
                  << " | traffic: "
                  << formatBytes(stat.bytes)
                  << "\n";

        if (++count >= 5)
            break;
    }

    // ---------------------------------------------------------
    // Top Destination IPs
    // ---------------------------------------------------------

    std::vector<std::pair<uint32_t, TrafficStat>> destinations(
        destination_ips_.begin(),
        destination_ips_.end()
    );

    std::sort(
        destinations.begin(),
        destinations.end(),
        [](const auto& a, const auto& b) {
            return a.second.bytes > b.second.bytes;
        }
    );

    std::cout << "\n[Top Destination IPs]\n";

    count = 0;

    for (const auto& [ip, stat] : destinations) {

        std::cout << "  "
                  << (count + 1)
                  << ". "
                  << std::setw(15)
                  << formatIP(ip)
                  << " | packets: "
                  << std::setw(6)
                  << stat.packets
                  << " | traffic: "
                  << formatBytes(stat.bytes)
                  << "\n";

        if (++count >= 5)
            break;
    }

    // ---------------------------------------------------------
    // Protocol Distribution
    // ---------------------------------------------------------

    std::cout << "\n[Protocol Distribution]\n";

    for (const auto& [protocol, stat] : protocols_) {

        std::string name;

        if (protocol == 6)
            name = "TCP";
        else if (protocol == 17)
            name = "UDP";
        else
            name = "Other";

        std::cout << "  "
                  << std::setw(8)
                  << name
                  << " | packets: "
                  << std::setw(6)
                  << stat.packets
                  << " | traffic: "
                  << formatBytes(stat.bytes)
                  << "\n";
    }

    // ---------------------------------------------------------
    // Top Applications
    // ---------------------------------------------------------

    std::vector<std::pair<AppType, TrafficStat>> apps(
        applications_.begin(),
        applications_.end()
    );

    std::sort(
        apps.begin(),
        apps.end(),
        [](const auto& a, const auto& b) {
            return a.second.bytes > b.second.bytes;
        }
    );

    std::cout << "\n[Top Applications]\n";

    count = 0;

    for (const auto& [app, stat] : apps) {

        std::cout << "  "
                  << (count + 1)
                  << ". "
                  << std::setw(15)
                  << appTypeToString(app)
                  << " | packets: "
                  << std::setw(6)
                  << stat.packets
                  << " | traffic: "
                  << formatBytes(stat.bytes)
                  << "\n";

        if (++count >= 5)
            break;
    }

    // ---------------------------------------------------------
    // Top Domains
    // ---------------------------------------------------------

    std::vector<std::pair<std::string, TrafficStat>> domains(
        domains_.begin(),
        domains_.end()
    );

    std::sort(
        domains.begin(),
        domains.end(),
        [](const auto& a, const auto& b) {
            return a.second.bytes > b.second.bytes;
        }
    );

    std::cout << "\n[Top Domains / SNIs]\n";

    count = 0;

    for (const auto& [domain, stat] : domains) {

        std::cout << "  "
                  << (count + 1)
                  << ". "
                  << std::setw(30)
                  << domain
                  << " | packets: "
                  << std::setw(6)
                  << stat.packets
                  << " | traffic: "
                  << formatBytes(stat.bytes)
                  << "\n";

        if (++count >= 5)
            break;
    }

    std::cout << "\n============================================================\n";
}

} // namespace DPI