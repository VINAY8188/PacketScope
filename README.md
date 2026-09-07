\# PacketScope



\### Multithreaded Deep Packet Inspection \& Network Traffic Analytics Engine



PacketScope is a C++17-based network traffic analysis and Deep Packet Inspection (DPI) system that processes network traffic captured in PCAP files.



It parses Ethernet, IPv4, TCP and UDP packets, tracks network flows using the five-tuple, extracts HTTP Host/TLS SNI information, classifies applications and domains, applies configurable blocking rules, and generates traffic analytics.



The project also includes a multithreaded processing pipeline using Load Balancers, Fast Path workers and thread-safe queues.



\---



\## 🚀 Features



\### Core DPI Features



\- PCAP file reading and packet processing

\- Ethernet and IPv4 packet parsing

\- TCP and UDP protocol detection

\- Five-tuple based flow identification

\- TLS Client Hello / SNI extraction

\- HTTP Host extraction

\- Application and domain classification

\- IP, application and domain blocking

\- Filtered PCAP output



\### PacketScope Enhancements



\#### 1. Network Traffic Analytics



PacketScope analyzes network traffic and reports:



\- Top source IPs

\- Top destination IPs

\- Protocol distribution

\- Top applications by traffic volume

\- Top domains / TLS SNI values

\- Packet count and byte usage



\#### 2. JSON Traffic Report



PacketScope can generate a machine-readable `traffic\_report.json` containing:



\- Total packets and bytes

\- TCP/UDP statistics

\- Forwarded and dropped packets

\- Application statistics

\- Source/destination IP statistics

\- Domain/SNI statistics

\- Protocol statistics



\#### 3. Performance Benchmarking



The engine measures:



\- Total processing time

\- Packets per second (PPS)

\- Throughput in MB/s

\- Average processing time per packet



\---



\## 🏗️ Architecture



```text

&#x20;                   ┌─────────────────┐

&#x20;                   │   PCAP Reader   │

&#x20;                   └────────┬────────┘

&#x20;                            │

&#x20;                            ▼

&#x20;                 ┌─────────────────────┐

&#x20;                 │  Load Balancers     │

&#x20;                 │  (Traffic Routing)  │

&#x20;                 └─────────┬───────────┘

&#x20;                           │

&#x20;            ┌──────────────┼──────────────┐

&#x20;            ▼              ▼              ▼

&#x20;       ┌─────────┐    ┌─────────┐    ┌─────────┐

&#x20;       │ Fast    │    │ Fast    │    │ Fast    │

&#x20;       │ Path    │    │ Path    │    │ Path    │

&#x20;       └────┬────┘    └────┬────┘    └────┬────┘

&#x20;            │              │              │

&#x20;            └──────────────┼──────────────┘

&#x20;                           ▼

&#x20;                ┌────────────────────┐

&#x20;                │ Traffic Analytics  │

&#x20;                │ \& Rule Processing  │

&#x20;                └─────────┬──────────┘

&#x20;                          │

&#x20;                   ┌──────┴──────┐

&#x20;                   ▼             ▼

&#x20;             Forwarded        Dropped

&#x20;                   │

&#x20;                   ▼

&#x20;             Output PCAP

Processing Pipeline

PCAP

&#x20; ↓

Packet Parsing

&#x20; ↓

Five-Tuple Flow Identification

&#x20; ↓

TLS SNI / HTTP Host Extraction

&#x20; ↓

Application Classification

&#x20; ↓

Blocking Rules

&#x20; ↓

Traffic Analytics

&#x20; ↓

Forward / Drop

&#x20; ↓

Output PCAP + JSON Report

🧵 Multithreaded Processing



PacketScope uses a producer-consumer architecture.



The processing pipeline consists of:



Reader thread

Load Balancer threads

Fast Path worker threads

Output Writer thread

Thread-safe queues



Packets are distributed using hashing of the five-tuple.



This helps ensure that packets belonging to the same network flow are consistently processed by the same Fast Path worker, allowing flow state to be maintained safely.



🔍 Deep Packet Inspection



PacketScope can inspect information beyond basic IP addresses and ports.



For HTTPS traffic, it attempts to extract the TLS Server Name Indication (SNI) from the Client Hello message.



Example:



TLS Client Hello

&#x20;      ↓

SNI: www.netflix.com

&#x20;      ↓

Application Classification

&#x20;      ↓

Netflix



Similarly, HTTP traffic can be inspected for the Host header.



🛡️ Traffic Blocking



PacketScope supports multiple rule types:



Rule	Example	Action

IP	192.168.1.50	Block source traffic

Application	YouTube	Block application traffic

Domain	facebook.com	Block matching domain



Once a flow is identified as blocked, subsequent packets belonging to that flow can be dropped.



📊 Network Traffic Analytics



PacketScope provides additional analytics after processing a PCAP file.



Example:



================ NETWORK TRAFFIC ANALYTICS ================



Top Source IPs:

&#x20; 192.168.1.100    45 packets    3210 bytes



Top Destination IPs:

&#x20; 142.250.185.206  20 packets    1800 bytes



Protocol Distribution:

&#x20; TCP              73 packets

&#x20; UDP               4 packets



Top Applications:

&#x20; HTTPS             39 packets

&#x20; Unknown           16 packets

&#x20; DNS                4 packets

&#x20; Netflix            1 packet

&#x20; Microsoft          1 packet

&#x20; Twitter/X          1 packet



The analytics module provides a quick overview of how network traffic is distributed across protocols, applications, domains and endpoints.



⚡ Performance Benchmark



PacketScope reports processing performance after each run.



Example:



================ PERFORMANCE BENCHMARK ================



Processing Time:       0.502108 seconds

Packets Per Second:    153.35 PPS

Throughput:            0.01 MB/s

Avg Time Per Packet:   6520.88 microseconds



Performance depends on the input PCAP size, packet contents, system hardware and configured worker threads.



📄 JSON Traffic Report



PacketScope can generate:



traffic\_report.json



Example:



{

&#x20; "total\_packets": 77,

&#x20; "total\_bytes": 5738,

&#x20; "tcp\_packets": 73,

&#x20; "udp\_packets": 4,

&#x20; "forwarded": 77,

&#x20; "dropped": 0

}



The report also contains application, protocol, source IP, destination IP and domain statistics.



📁 Project Structure

PacketScope/

│

├── include/

│   ├── connection\_tracker.h

│   ├── dpi\_engine.h

│   ├── fast\_path.h

│   ├── load\_balancer.h

│   ├── packet\_parser.h

│   ├── pcap\_reader.h

│   ├── platform.h

│   ├── rule\_manager.h

│   ├── sni\_extractor.h

│   ├── thread\_safe\_queue.h

│   ├── traffic\_analytics.h

│   └── types.h

│

├── src/

│   ├── connection\_tracker.cpp

│   ├── dpi\_engine.cpp

│   ├── dpi\_mt.cpp

│   ├── fast\_path.cpp

│   ├── load\_balancer.cpp

│   ├── main.cpp

│   ├── main\_dpi.cpp

│   ├── main\_simple.cpp

│   ├── main\_working.cpp

│   ├── packet\_parser.cpp

│   ├── pcap\_reader.cpp

│   ├── rule\_manager.cpp

│   ├── sni\_extractor.cpp

│   ├── traffic\_analytics.cpp

│   └── types.cpp

│

├── generate\_test\_pcap.py

├── test\_dpi.pcap

├── TRAFFIC\_ANALYTICS.md

├── WINDOWS\_SETUP.md

├── CMakeLists.txt

└── README.md

🛠️ Technologies Used

C++17

CMake

TCP/IP Networking

PCAP

Multithreading

TLS / SNI

HTTP

TCP / UDP

Python for test PCAP generation

Git / GitHub

💻 Build \& Run

Requirements

C++17 compatible compiler

CMake (optional)

Python 3 for generating test traffic

Build with g++

g++ -std=c++17 -pthread -O2 -I include \\

&#x20;   -o dpi\_engine \\

&#x20;   src/dpi\_mt.cpp \\

&#x20;   src/pcap\_reader.cpp \\

&#x20;   src/packet\_parser.cpp \\

&#x20;   src/sni\_extractor.cpp \\

&#x20;   src/types.cpp

Windows PowerShell

g++ -std=c++17 -pthread -O2 -I include -o dpi\_engine.exe src/dpi\_mt.cpp src/pcap\_reader.cpp src/packet\_parser.cpp src/sni\_extractor.cpp src/types.cpp

Run



Linux/macOS:



./dpi\_engine test\_dpi.pcap output.pcap



Windows PowerShell:



.\\dpi\_engine.exe test\_dpi.pcap output.pcap

⚙️ Configure Threads



Example:



./dpi\_engine input.pcap output.pcap --lbs 4 --fps 4



This configures:



4 Load Balancer threads

×

4 Fast Path workers

=

16 processing workers

🧪 Generate Test Traffic



The project includes a Python script for generating test PCAP data.



python generate\_test\_pcap.py



This generates:



test\_dpi.pcap



which can then be processed by PacketScope.



📈 Example Processing Results



A sample test run processed:



Total Packets:       77

Total Bytes:         5738

TCP Packets:         73

UDP Packets:          4



Forwarded:           77

Dropped:              0



Detected application/domain examples include:



www.netflix.com  → Netflix

www.microsoft.com → Microsoft

twitter.com       → Twitter/X

🎯 Key Concepts Demonstrated



PacketScope demonstrates practical concepts in:



Network protocol parsing

Deep Packet Inspection

Flow-based traffic classification

TLS SNI extraction

HTTP Host extraction

Stateful traffic filtering

Multithreaded programming

Producer-consumer architecture

Thread-safe data structures

Network traffic analytics

Performance measurement

JSON-based reporting

🔮 Future Improvements



Possible extensions include:



Real-time packet capture

Live traffic dashboard

More application signatures

QUIC / HTTP3 support

Bandwidth throttling

Persistent rule configuration

Advanced traffic classification

CSV report export

Real-time monitoring

Graphical analytics dashboard

📚 Documentation



Additional documentation is available in:



TRAFFIC\_ANALYTICS.md

WINDOWS\_SETUP.md

📌 Project Note



PacketScope is an extended version of an existing C++ packet analysis / DPI project.



The project has been enhanced with:



Network traffic analytics

JSON reporting

Performance benchmarking

Improved application/domain classification



The original project's license and attribution requirements should be preserved where applicable.



👨‍💻 Author



Vinay Patidar



B.Tech — Computer Science \& Business Systems

