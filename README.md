# PacketScope

### Multithreaded Deep Packet Inspection & Network Traffic Analysis Engine

PacketScope is a **C++17-based Deep Packet Inspection (DPI) and network traffic analysis engine** designed to process PCAP files, inspect network packets, identify applications and domains, apply traffic rules, and generate detailed analytics.

The project uses a **multithreaded Load Balancer + Fast Path architecture** to distribute packet processing across worker threads.



## 🚀 Features

* 📦 PCAP packet reading
* 🌐 Ethernet and IPv4 packet parsing
* 🔌 TCP and UDP protocol analysis
* 🔗 Five-tuple flow identification
* 🔍 TLS SNI extraction
* 🌍 HTTP Host extraction
* 📱 Application and domain classification
* 🚫 Configurable IP, application, and domain blocking
* 📊 Network traffic analytics
* 🧵 Multithreaded packet processing
* ⚡ Performance benchmarking
* 📄 JSON traffic reports
* 💾 Filtered output PCAP generation
* 🔬 Wireshark-compatible output



## 🏗️ Architecture


                         INPUT PCAP
                             │
                             ▼
                      ┌─────────────┐
                      │ PCAP Reader │
                      └──────┬──────┘
                             │
                             ▼
                     ┌──────────────┐
                     │Packet Parser │
                     └──────┬───────┘
                            │
                            ▼
                   ┌──────────────────┐
                   │ Five-Tuple Flow  │
                   │   Identification │
                   └────────┬─────────┘
                            │
                            ▼
                    ┌───────────────┐
                    │ Load Balancers │
                    └───────┬───────┘
                            │
                    ┌───────┴───────┐
                    ▼               ▼
                 LB0               LB1
                  │                 │
               ┌──┴──┐           ┌──┴──┐
               ▼     ▼           ▼     ▼
              FP0   FP1         FP2   FP3
               │     │           │     │
               └─────┴─────┬─────┴─────┘
                           ▼
                  DPI / Application
                     Classification
                           │
                 ┌─────────┴─────────┐
                 ▼                   ▼
              BLOCK                 ALLOW
                 │                   │
                DROP              FORWARD
                 └─────────┬─────────┘
                           ▼
                    Traffic Analytics
                           │
                 ┌─────────┴─────────┐
                 ▼                   ▼
             output.pcap       traffic_report.json




## 🔄 Packet Processing Pipeline


PCAP
 ↓
PCAP Reader
 ↓
Ethernet Parsing
 ↓
IPv4 Parsing
 ↓
TCP / UDP Parsing
 ↓
Five-Tuple Identification
 ↓
Load Balancing
 ↓
Fast Path Workers
 ↓
TLS SNI / HTTP Host Extraction
 ↓
Application Classification
 ↓
Traffic Rules
 ↓
Forward / Drop
 ↓
Analytics
 ↓
Output PCAP + JSON Report




## 🧵 Multithreaded Processing

PacketScope uses multiple processing stages to distribute packet-processing work.

### Configuration

| Component         | Count |
| ----------------- | ----- |
| Load Balancers    |     2 |
| Fast Path Workers |     4 |
| Total Workers     |     4 |

Packets are distributed by the Load Balancers and processed by Fast Path worker threads.

The architecture follows a producer-consumer style design using thread-safe queues.



## 🔍 Deep Packet Inspection

PacketScope performs inspection beyond basic IP and port information.

### Protocol Information

The parser handles:

* Ethernet
* IPv4
* TCP
* UDP

### Flow Identification

Flows can be identified using the five-tuple:


Source IP
Destination IP
Source Port
Destination Port
Protocol


Example:


192.168.1.100:52341
        ↓
142.250.185.206:443
        ↓
TCP




## 🔐 TLS / HTTPS Inspection

PacketScope does **not decrypt HTTPS traffic**.

Instead, it can inspect available TLS metadata such as the **Server Name Indication (SNI)** from the TLS ClientHello.

Example:

www.facebook.com
www.instagram.com
www.youtube.com
github.com
www.netflix.com


This allows the engine to associate encrypted traffic with domains or applications without decrypting the actual HTTPS payload.



## 🌐 HTTP Host Detection

For HTTP traffic, PacketScope can inspect the HTTP Host information.

Example:

Host: example.com


The extracted host can then be used for classification and traffic analysis.



## 📱 Application Classification

PacketScope identifies applications and domains using available packet metadata such as:

* Protocol
* Ports
* TLS SNI
* HTTP Host
* Packet characteristics

Example applications detected during testing:

Facebook
Spotify
TikTok
Telegram
Netflix
Amazon
Cloudflare
Twitter/X
Instagram
Discord
Zoom
YouTube
Microsoft
Apple
Google
GitHub




# 📊 Test Results

The following results were obtained from a test PCAP containing **77 packets**.

## Packet Statistics

| Metric        | Result |
| ------------- | -----: |
| Total Packets |     77 |
| Total Bytes   |   5738 |
| TCP Packets   |     73 |
| UDP Packets   |      4 |
| Forwarded     |     77 |
| Dropped       |      0 |

> No packet matched a blocking rule in this particular test, so all 77 packets were forwarded.



## 🧵 Thread Statistics

| Worker | Packets |
| ------ | ------: |
| LB0    |      53 |
| LB1    |      24 |
| FP0    |      53 |
| FP1    |       0 |
| FP2    |       0 |
| FP3    |      24 |

The test demonstrates packet distribution across the configured Load Balancers and Fast Path workers.



## 📱 Application Breakdown

| Application | Packets | Percentage |
| ----------- | ------: | ---------: |
| HTTPS       |      39 |      50.6% |
| Unknown     |      16 |      20.8% |
| DNS         |       4 |       5.2% |
| HTTP        |       2 |       2.6% |

Additional applications detected:


Facebook
Spotify
TikTok
Telegram
Netflix
Amazon
Cloudflare
Twitter/X
Instagram
Discord
Zoom
YouTube
Microsoft
Apple
Google
GitHub




## 🌍 Detected Domains / SNI


httpbin.org          → HTTPS
zoom.us              → Zoom
www.youtube.com      → YouTube
www.facebook.com     → Facebook
www.instagram.com    → Instagram
example.com          → HTTPS
open.spotify.com     → Spotify
www.google.com       → Google
www.amazon.com       → Amazon
web.telegram.org     → Telegram
discord.com          → Discord
www.cloudflare.com   → Cloudflare
www.netflix.com      → Netflix
www.tiktok.com       → TikTok
github.com           → GitHub
www.microsoft.com    → Microsoft
twitter.com          → Twitter/X
www.apple.com        → Apple




# 📈 Network Traffic Analytics

## Top Source IPs

| Rank | Source IP       | Packets | Traffic |
| ---: | --------------- | ------: | ------: |
|    1 | 192.168.1.100   |      56 | 4.50 KB |
|    2 | 192.168.1.50    |       5 |   270 B |
|    3 | 52.94.236.248   |       1 |    54 B |
|    4 | 142.250.185.206 |       1 |    54 B |
|    5 | 17.253.144.10   |       1 |    54 B |

## Top Destination IPs

| Rank | Destination IP | Packets | Traffic |
| ---: | -------------- | ------: | ------: |
|    1 | 192.168.1.100  |      16 |   864 B |
|    2 | 8.8.8.8        |       4 |   300 B |
|    3 | 172.217.0.100  |       5 |   270 B |
|    4 | 192.0.78.24    |       3 |   250 B |
|    5 | 157.240.1.174  |       3 |   249 B |

## Protocol Distribution

| Protocol | Packets | Traffic |
| -------- | ------: | ------: |
| TCP      |      73 | 5.31 KB |
| UDP      |       4 |   300 B |



# ⚡ Performance

Performance metrics from the same 77-packet test:

| Metric                |           Result |
| --------------------- | ---------------: |
| Processing Time       | 0.509782 seconds |
| Packets Per Second    |       151.04 PPS |
| Throughput            |        0.01 MB/s |
| Average Time / Packet |       6620.55 µs |

> **Note:** These numbers are from a small functional test PCAP and should not be considered a production-scale benchmark.



# 📄 JSON Report

PacketScope automatically generates:


traffic_report.json

The report contains traffic statistics and analysis results generated during processing.

Example:


{
  "total_packets": 77,
  "total_bytes": 5738,
  "tcp_packets": 73,
  "udp_packets": 4,
  "forwarded": 77,
  "dropped": 0
}



# 💾 Output PCAP

After processing, PacketScope generates:


output

The output PCAP can be opened using **Wireshark** for packet-level inspection and verification.


Input PCAP
     ↓
PacketScope
     ↓
output.pcap
     ↓
Wireshark




# 🛠️ Technologies

| Technology     | Purpose                         |
| -------------- | ------------------------------- |
| C++17          | Core implementation             |
| GCC / MinGW    | Compilation                     |
| MSYS2 UCRT64   | Windows development environment |
| PCAP           | Packet capture input/output     |
| Multithreading | Parallel packet processing      |
| JSON           | Traffic reporting               |
| Wireshark      | Packet verification             |
| Git / GitHub   | Version control                 |



# 📁 Project Structure


PacketScope/
│
├── include/
│   ├── connection_tracker.h
│   ├── dpi_engine.h
│   ├── fast_path.h
│   ├── load_balancer.h
│   ├── packet_parser.h
│   ├── pcap_reader.h
│   ├── platform.h
│   ├── rule_manager.h
│   ├── sni_extractor.h
│   ├── thread_safe_queue.h
│   ├── traffic_analytics.h
│   └── types.h
│
├── src/
│   ├── connection_tracker.cpp
│   ├── dpi_engine.cpp
│   ├── dpi_mt.cpp
│   ├── fast_path.cpp
│   ├── load_balancer.cpp
│   ├── main.cpp
│   ├── main_dpi.cpp
│   ├── main_simple.cpp
│   ├── main_working.cpp
│   ├── packet_parser.cpp
│   ├── pcap_reader.cpp
│   ├── rule_manager.cpp
│   ├── sni_extractor.cpp
│   ├── traffic_analytics.cpp
│   └── types.cpp
│
├── test_dpi.pcap
├── traffic_report.json
├── output.pcap
├── generate_test_pcap.py
├── CMakeLists.txt
├── README.md
├── TRAFFIC_ANALYTICS.md
└── WINDOWS_SETUP.md



# ▶️ Build and Run

## 1. Verify GCC

```powershell
where.exe g++
g++ --version
```

The project was tested using the MSYS2 UCRT64 GCC environment.


## 2. Build

Run:

```powershell
g++ -std=c++17 -pthread -O2 -I include -o dpi_engine.exe src/dpi_mt.cpp src/pcap_reader.cpp src/packet_parser.cpp src/sni_extractor.cpp src/types.cpp
```



## 3. Run PacketScope

```powershell
.\dpi_engine.exe test_dpi.pcap output.pcap
```



## 4. Check Generated Files

```powershell
dir traffic_report.json
dir output.pcap
```

View the JSON report:

```powershell
Get-Content traffic_report.json
```



# 🧪 Test Traffic

A test PCAP can be generated using:

```powershell
python generate_test_pcap.py
```

The generated PCAP can then be processed by PacketScope.



# 🚫 Traffic Filtering

PacketScope supports configurable traffic filtering based on rules such as:

* IP address
* Application
* Domain

Conceptually:


Packet
  ↓
Classification
  ↓
Rule Matching
  ↓
 ┌───────────────┐
 │ Rule Matched? │
 └───────┬───────┘
         │
    ┌────┴────┐
    ▼         ▼
   YES        NO
    │          │
   DROP      FORWARD




# 📚 Key Concepts

### Deep Packet Inspection

Examines packet headers and available application-layer metadata to identify traffic.

### Five-Tuple

Identifies a network flow using:


Source IP
Destination IP
Source Port
Destination Port
Protocol


### SNI

TLS metadata that can expose the requested server/domain without decrypting HTTPS traffic.

### Load Balancer

Distributes packets between processing paths.

### Fast Path

Worker-processing stage responsible for packet inspection and classification.

### Traffic Analytics

Collects information such as:

* Packet counts
* Byte counts
* Protocol distribution
* Source IPs
* Destination IPs
* Applications
* Domains
* Processing performance



# 🔮 Future Improvements

Possible future improvements include:

* Real-time network interface capture
* More application signatures
* Improved flow tracking
* More advanced filtering rules
* Better load-balancing strategies
* Performance optimization
* Expanded protocol support
* Real-time dashboards
* More extensive benchmarking
* Additional security analysis features



# 📖 Documentation

Additional project documentation:

```text
WINDOWS_SETUP.md
TRAFFIC_ANALYTICS.md
```



# ⚠️ Project Note

PacketScope is an extended/adapted C++ packet-analysis and DPI project. The work involved setting up the development environment, working with the multithreaded DPI pipeline, configuring and testing the system, working with classification/analytics/reporting/filtering functionality, and validating generated PCAP and JSON results.


# 👨‍💻 Author

**Vinay**

B.Tech Student
C++ | Networking | Deep Packet Inspection | Network Traffic Analysis

---

## ⭐ Project Summary


                 PacketScope
                      │
                      ▼
                  PCAP Input
                      │
                      ▼
                Packet Parsing
                      │
                      ▼
              Flow Identification
                      │
                      ▼
               Load Balancing
                      │
                      ▼
              Multithreaded DPI
                      │
             ┌────────┴────────┐
             ▼                 ▼
       Application          Domain/SNI
       Classification        Detection
             │                 │
             └────────┬────────┘
                      ▼
                Traffic Rules
                      │
                ┌─────┴─────┐
                ▼           ▼
              DROP        FORWARD
                └─────┬─────┘
                      ▼
                Traffic Analytics
                      │
              ┌───────┴────────┐
              ▼                ▼
         output.pcap    traffic_report.json
