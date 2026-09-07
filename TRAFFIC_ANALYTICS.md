# Traffic Analytics — Added Feature

This version extends the original multi-threaded DPI engine with network traffic analytics.

## New analytics

- Top 5 source IPs by traffic volume
- Top 5 destination IPs by traffic volume
- Protocol distribution with packet and byte counts
- Top 5 applications by traffic volume
- Top 5 detected domains/SNIs by traffic volume
- Human-readable byte formatting (B / KB / MB)

## Implementation

The analytics are collected inside the existing Fast Path processing stage. A mutex protects the shared analytics maps because multiple Fast Path threads update them concurrently.

The existing DPI classification and blocking behavior are unchanged.

## Build on Windows

```powershell
g++ -std=c++17 -pthread -O2 -I include -o dpi_engine.exe src/dpi_mt.cpp src/pcap_reader.cpp src/packet_parser.cpp src/sni_extractor.cpp src/types.cpp
```

## Run

```powershell
.\dpi_engine.exe test_dpi.pcap output.pcap
```

The processing report will now include a `NETWORK TRAFFIC ANALYTICS` section.
