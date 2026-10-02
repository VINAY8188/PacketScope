# 🚀 PacketScope

### Multithreaded Deep Packet Inspection & Network Traffic Analytics Engine





\

> **PacketScope is a C++17-based multithreaded Deep Packet Inspection (DPI) and network traffic analytics engine designed to analyze captured network traffic, identify protocols and applications, extract domains from HTTP/TLS metadata, apply traffic filtering rules, and generate detailed reports.**

---

## 📌 Overview

PacketScope processes network traffic captured in **PCAP files** and performs analysis across multiple protocol layers.

The engine can:

* Parse Ethernet and IPv4 packets
* Identify TCP and UDP traffic
* Track flows using the **five-tuple**
* Extract **TLS SNI** from TLS ClientHello messages
* Extract **HTTP Host** information
* Classify applications and domains
* Apply IP, application and domain filtering rules
* Process packets using a multithreaded architecture
* Generate network traffic analytics
* Measure processing performance
* Generate a machine-readable JSON report
* Write processed packets to an output PCAP

---

# ✨ Key Highlights

| Capability                | Description                                                     |
| ------------------------- | --------------------------------------------------------------- |
| 🔍 Deep Packet Inspection | Inspects packet headers and selected application-layer metadata |
| 🧵 Multithreading         | Uses Load Balancers and Fast Path worker threads                |
| 🌐 Protocol Analysis      | Ethernet, IPv4, TCP and UDP parsing                             |
| 🔐 TLS Analysis           | Extracts Server Name Indication (SNI) from TLS ClientHello      |
| 🌍 Domain Detection       | Identifies domains from TLS SNI / HTTP Host                     |
| 📊 Traffic Analytics      | Source IP, destination IP, protocol and application statistics  |
| 🛡️ Traffic Filtering     | Supports IP, application and domain-based rules                 |
| 📄 JSON Reporting         | Generates `traffic_report.json`                                 |
| 📦 PCAP Output            | Generates processed `output.pcap`                               |
| ⚡ Performance Metrics     | Processing time, PPS, throughput and average packet time        |

---

# 🏗️ System Architecture

```text
                         INPUT PCAP
                             │
                             ▼
                    ┌─────────────────┐
                    │   PCAP Reader   │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │ Packet Parser   │
                    │ Ethernet / IP   │
                    │ TCP / UDP       │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │ Five-Tuple Flow │
                    │ Identification  │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │ Load Balancers  │
                    └───────┬─┬───────┘
                            │ │
                    ┌───────┘ └───────┐
                    ▼                 ▼
                ┌────────┐        ┌────────┐
                │  LB0   │        │  LB1   │
                └───┬────┘        └───┬────┘
                    │                 │
              ┌─────┴─────┐     ┌─────┴─────┐
              ▼           ▼     ▼           ▼
            ┌────┐      ┌────┐ ┌────┐      ┌────┐
            │ FP0│      │ FP1│ │ FP2│      │ FP3│
            └──┬─┘      └─┬──┘ └─┬──┘      └─┬──┘
               └──────────┴───────┴──────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │       DPI       │
                    │  Classification │
                    └────────┬────────┘
                             │
                  ┌──────────┴──────────┐
                  ▼                     ▼
          TLS SNI / HTTP Host      Rule Matching
                  │                     │
                  ▼                     ▼
          Application / Domain    Forward / Drop
             Classification              │
                  │                     │
                  └──────────┬──────────┘
                             ▼
                    ┌─────────────────┐
                    │ Traffic         │
                    │ Analytics       │
                    └────────┬────────┘
                             │
                 ┌───────────┴───────────┐
                 ▼                       ▼
          traffic_report.json       output.pcap
```

---

# 🔄 Packet Processing Pipeline

```text
PCAP File
   ↓
Packet Reading
   ↓
Ethernet Parsing
   ↓
IPv4 Parsing
   ↓
TCP / UDP Detection
   ↓
Five-Tuple Flow Identification
   ↓
Load Balancing
   ↓
Fast Path Worker Processing
   ↓
TLS SNI / HTTP Host Extraction
   ↓
Application & Domain Classification
   ↓
Traffic Rule Processing
   ↓
Forward / Drop
   ↓
Traffic Analytics
   ↓
JSON Report + Output PCAP
```

---

# 🧵 Multithreaded Processing

PacketScope follows a **producer-consumer style architecture**.

### Processing components

```text
PCAP Reader
     │
     ▼
Thread-Safe Queues
     │
     ▼
Load Balancers
     │
     ▼
Fast Path Workers
     │
     ▼
Output / Analytics
```

The project uses:

* Reader processing
* Load Balancer threads
* Fast Path worker threads
* Thread-safe queues
* Output processing

Packets can be distributed using their flow information so packets belonging to the same network flow can consistently reach the same worker.

### Example configuration

```text
Load Balancers: 2
Fast Path workers per LB: 2
Total Fast Path workers: 4
```

---

# 🔍 Deep Packet Inspection

PacketScope goes beyond basic IP and port inspection.

For example:

```text
Ethernet
   ↓
IPv4
   ↓
TCP
   ↓
TLS
   ↓
TLS ClientHello
   ↓
SNI
   ↓
Application Classification
```

### TLS SNI Example

```text
TLS ClientHello
       ↓
SNI: www.youtube.com
       ↓
Application Classification
       ↓
YouTube
```

PacketScope uses TLS metadata such as **Server Name Indication (SNI)** for domain/application identification.

> **Important:** PacketScope does not decrypt HTTPS application data. It uses available protocol metadata such as TLS SNI.

---

# 🌐 HTTP Host Extraction

For HTTP traffic, PacketScope can inspect the HTTP Host header.

Example:

```text
GET / HTTP/1.1
Host: example.com
```

The extracted hostname can then be used for domain/application classification.

---

# 🛡️ Traffic Filtering

PacketScope supports configurable traffic filtering.

### Supported rule categories

| Rule Type   | Example        | Action                    |
| ----------- | -------------- | ------------------------- |
| IP          | `192.168.1.50` | Block matching traffic    |
| Application | `YouTube`      | Block application traffic |
| Domain      | `facebook.com` | Block matching domain     |

Conceptually:

```text
Packet
  ↓
Classification
  ↓
Rule Matching
  ↓
Blocked?
 ┌───────┴───────┐
 │               │
YES              NO
 │               │
DROP           FORWARD
```

---

# 📊 Network Traffic Analytics

PacketScope generates traffic statistics after processing the PCAP.

The analytics include:

* Top source IPs
* Top destination IPs
* Protocol distribution
* Top applications
* Top domains / SNI values
* Packet counts
* Byte usage
* Forwarded packets
* Dropped packets

---

# 🧪 Latest Test Run

PacketScope was tested using:

```text
Input:
test_dpi.pcap
```

### PCAP Information

```text
PCAP Version : 2.4
Snaplen      : 65535 bytes
Link Type    : Ethernet
```

### Processing Summary

```text
Total Packets : 77
Total Bytes   : 5738
TCP Packets   : 73
UDP Packets   : 4

Forwarded     : 77
Dropped       : 0
```

---

# 🧵 Thread Statistics

The latest test used:

```text
Load Balancers : 2
Fast Paths     : 4
```

Observed processing:

```text
LB0 dispatched : 53 packets
LB1 dispatched : 24 packets

FP0 processed  : 53 packets
FP1 processed  : 0 packets
FP2 processed  : 0 packets
FP3 processed  : 24 packets
```

Total:

```text
53 + 24 = 77 packets
```

---

# 📱 Application Classification

Latest test results:

```text
HTTPS        39 packets   50.6%
Unknown      16 packets   20.8%
DNS           4 packets    5.2%
HTTP          2 packets    2.6%
Facebook      1 packet     1.3%
Spotify       1 packet     1.3%
TikTok        1 packet     1.3%
Telegram      1 packet     1.3%
Netflix       1 packet     1.3%
Amazon        1 packet     1.3%
Cloudflare    1 packet     1.3%
Twitter/X     1 packet     1.3%
Instagram     1 packet     1.3%
Discord       1 packet     1.3%
Zoom          1 packet     1.3%
YouTube       1 packet     1.3%
Microsoft     1 packet     1.3%
Apple         1 packet     1.3%
Google        1 packet     1.3%
GitHub        1 packet     1.3%
```

---

# 🌍 Detected Domains / SNI

The test traffic produced domain/SNI detections including:

```text
httpbin.org            → HTTPS
zoom.us                → Zoom
www.youtube.com        → YouTube
www.facebook.com       → Facebook
www.instagram.com      → Instagram
example.com            → HTTPS
open.spotify.com       → Spotify
www.google.com         → Google
www.amazon.com         → Amazon
web.telegram.org       → Telegram
discord.com            → Discord
www.cloudflare.com     → Cloudflare
www.netflix.com        → Netflix
www.tiktok.com         → TikTok
github.com             → GitHub
www.microsoft.com      → Microsoft
twitter.com            → Twitter/X
www.apple.com          → Apple
```

---

# 📈 Traffic Analytics — Latest Run

### Top Source IPs

```text
1. 192.168.1.100  | 56 packets | 4.50 KB
2. 192.168.1.50   |  5 packets | 270 B
3. 52.94.236.248  |  1 packet  | 54 B
4. 142.250.185.206|  1 packet  | 54 B
5. 17.253.144.10  |  1 packet  | 54 B
```

### Top Destination IPs

```text
1. 192.168.1.100  | 16 packets | 864 B
2. 8.8.8.8        |  4 packets | 300 B
3. 172.217.0.100  |  5 packets | 270 B
4. 192.0.78.24    |  3 packets | 250 B
5. 157.240.1.174  |  3 packets | 249 B
```

### Protocol Distribution

```text
TCP | 73 packets | 5.31 KB
UDP |  4 packets | 300 B
```

### Top Applications by Traffic

```text
1. HTTPS       | 39 packets | 2.21 KB
2. Unknown     | 16 packets | 864 B
3. DNS         |  4 packets | 300 B
4. Cloudflare  |  1 packet  | 142 B
5. Instagram   |  1 packet  | 141 B
```

---

# ⚡ Performance Benchmark

Latest measured run:

```text
Processing Time     : 0.509782 seconds
Packets Per Second  : 151.04 PPS
Throughput          : 0.01 MB/s
Avg Time Per Packet : 6620.55 microseconds
```

> **Note:** This benchmark uses a small 77-packet test PCAP. Performance will vary depending on PCAP size, packet contents, system hardware and worker configuration. These numbers should not be interpreted as production-scale throughput.

---

# 📄 JSON Traffic Report

PacketScope generates:

```text
traffic_report.json
```

Example:

```json
{
  "total_packets": 77,
  "total_bytes": 5738,
  "tcp_packets": 73,
  "udp_packets": 4,
  "forwarded": 77,
  "dropped": 0
}
```

The report also contains detailed:

* Application statistics
* Protocol statistics
* Source IP statistics
* Destination IP statistics
* Domain/SNI statistics
* Traffic totals
* Forward/drop information

---

# 📦 Output PCAP

PacketScope writes the processed traffic to:

```text
output.pcap
```

The output PCAP can be inspected using tools such as **Wireshark**.

### Workflow

```text
test_dpi.pcap
      ↓
  PacketScope
      ↓
 output.pcap
      ↓
   Wireshark
```

This makes it possible to independently inspect the packet-level output.

---

# 📁 Project Structure

```text
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
├── generate_test_pcap.py
├── test_dpi.pcap
├── TRAFFIC_ANALYTICS.md
├── WINDOWS_SETUP.md
├── CMakeLists.txt
└── README.md
```

---

# 🛠️ Technologies Used

| Technology         | Purpose                                 |
| ------------------ | --------------------------------------- |
| **C++17**          | Core implementation                     |
| **CMake**          | Build configuration                     |
| **PCAP**           | Packet capture input/output             |
| **TCP/IP**         | Network protocol analysis               |
| **TCP / UDP**      | Transport-layer processing              |
| **TLS / SNI**      | Domain identification from TLS metadata |
| **HTTP**           | Host-header inspection                  |
| **Multithreading** | Parallel packet processing              |
| **Python**         | Test PCAP generation                    |
| **JSON**           | Machine-readable reporting              |
| **Wireshark**      | Packet inspection and validation        |
| **Git / GitHub**   | Version control and project hosting     |

---

# 💻 Build & Run

## Requirements

* C++17-compatible compiler
* CMake (optional)
* Python 3 for test PCAP generation
* Git

### Windows

This project has been tested using **MSYS2 UCRT64 / GCC**.

Check the compiler:

```powershell
g++ --version
```

### Build

```powershell
g++ -std=c++17 -pthread -O2 -I include -o dpi_engine.exe src/dpi_mt.cpp src/pcap_reader.cpp src/packet_parser.cpp src/sni_extractor.cpp src/types.cpp
```

### Run

```powershell
.\dpi_engine.exe test_dpi.pcap output.pcap
```

The program generates:

```text
traffic_report.json
output.pcap
```

---

# 🧪 Generate Test Traffic

The project includes a Python script for generating test PCAP data:

```powershell
python generate_test_pcap.py
```

This generates:

```text
test_dpi.pcap
```

which can then be processed by PacketScope.

---

# ⚙️ Configure Processing Threads

Example:

```text
./dpi_engine input.pcap output.pcap --lbs 4 --fps 4
```

Conceptually this configures:

```text
4 Load Balancers
        ×
4 Fast Path workers per LB
        =
16 Fast Path processing workers
```

> Use the thread configuration supported by the current implementation/build.

---

# 🎯 Key Concepts Demonstrated

PacketScope demonstrates practical concepts in:

* Network protocol parsing
* Deep Packet Inspection
* Five-tuple flow identification
* TLS SNI extraction
* HTTP Host extraction
* Application classification
* Domain classification
* Stateful traffic filtering
* Multithreaded programming
* Producer-consumer architecture
* Thread-safe queues
* Load balancing
* Network traffic analytics
* Performance measurement
* JSON-based reporting
* PCAP processing

---

# 🔮 Future Improvements

Possible future extensions include:

* Real-time packet capture
* Live traffic dashboard
* More application signatures
* QUIC / HTTP/3 support
* Bandwidth throttling
* Persistent rule configuration
* Advanced traffic classification
* CSV report export
* Real-time monitoring
* Graphical analytics dashboard

---

# 📚 Documentation

Additional documentation:

* `TRAFFIC_ANALYTICS.md`
* `WINDOWS_SETUP.md`

---

# 📌 Project Note

PacketScope is an **extended version of an existing C++ packet analysis / DPI project**.

The project has been enhanced with:

* Network traffic analytics
* JSON reporting
* Performance benchmarking
* Improved application/domain classification
* Multithreaded processing and analysis

The original project's license and attribution requirements should be preserved where applicable.

---

# 👨‍💻 Author

### Vinay Patidar

**B.Tech — Computer Science & Business Systems**

---

## ⭐ Project Summary

```text
PCAP
 ↓
Packet Parsing
 ↓
Flow Identification
 ↓
Multithreaded Processing
 ↓
Deep Packet Inspection
 ↓
Application / Domain Classification
 ↓
Traffic Filtering
 ↓
Analytics
 ↓
JSON Report + Output PCAP
```

**PacketScope — Analyze. Classify. Filter. Understand Network Traffic.**
