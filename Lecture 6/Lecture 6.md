## Objectives
- Work with the Bluetooth API on the ESP32-S3
- Configure and use Wi-Fi connectivity on the ESP32-S3

## Bluetooth
### Introduction
Up to this point, we have explored wired communication protocols like **UART, I²C, and SPI**. These protocols are essential tools in an embedded system designer's toolkit. They allow us to expand our microcontroller's capabilities, add external sensors, interface with displays, and connect multiple microcontrollers together over a shared bus.

However, wired communication introduces significant limitations:

- **Lack of Mobility:** Devices must remain physically tethered together.
    
- **Short Range:** Protocols like I²C and SPI are designed for short distances (typically on the same circuit board). Extending them over long wires introduces noise and signal degradation.
    
- **Scalability & Cost:** Running physical wires between dozens of environmental sensors scattered across a building is expensive and impractical.

Wireless communication protocols solve these problems by transmitting and receiving data using Radio Frequency (RF) waves through the air. This eliminates the need for physical cables, enabling true mobility, longer ranges, and easier deployment of complex sensor networks. The two most common wireless protocols in the embedded space are Wi-Fi great for high-bandwidth internet connectivity and Bluetooth ideal for low-power, device-to-device communication.

### The Bluetooth Protocol
Bluetooth is a short-range wireless communication standard that operates in the 2.4 GHz ISM (Industrial, Scientific, and Medical) radio band. It is designed to exchange data between devices over short distances using low-power radio communication.

Instead of transmitting a continuous signal, Bluetooth divides data into small units called packets. These packets are transmitted over the air using a technique called frequency hopping, where the radio rapidly switches between different frequencies within the 2.4 GHz band. This reduces interference from other devices such as Wi-Fi routers and microwaves that share the same band.

The 2.4 GHz band is divided into multiple channels. Bluetooth Classic uses 79 channels, each spaced 1 MHz apart, while Bluetooth Low Energy (BLE) uses 40 channels, each spaced 2 MHz apart. During communication, devices follow a shared hopping sequence and transmit each packet on a different channel in that sequence.

To exchange data, Bluetooth generally relies on a Central/Peripheral architecture:
- **Peripheral (Server):** A device that advertises its presence and holds the data.
- **Central (Client):** A device that scans for peripherals, connects to them, and requests or sends data.

### Type Of Bluetooth


### Bluetooth Network Architecture
#### Piconets
When devices connect via Bluetooth, they form a tiny, ad-hoc network called a **Piconet**. In a piconet, devices take on specific roles:
1. **Master:** The device that initiates the connection and dictates the frequency-hopping sequence and timing. (e.g., your smartphone).
2. **Slave:** The devices that follow the Master's rules and synchronize to its clock. (e.g., your wireless earbuds and your smartwatch).

A single piconet can have one Master and up to seven active Slaves connected at the same time. The Master talks to the Slaves, but the Slaves do not talk directly to each other; all information routes through the Master.

<img src="./attachments/network1.png" />

#### Scatternets
A scatternet is formed when multiple piconets are connected through devices that participate in more than one piconet, allowing a larger network to function seamlessly.

<img src="./attachments/network2.png" />

In a scatternet, a device can be a Slave in one piconet but act as a Master in another, creating a web of interconnected wireless nodes.
#### Connection Process
For two devices to form network and share data, they go through a specific connection process:
- **Inquiry:** If a device wants to find other devices, it sends out an inquiry request. A device that wants to be found must be in "discoverable mode," meaning it actively listens for and responds to these requests.
- **Paging:** Once the devices have found each other, they form a connection. This involves synchronizing their clocks and agreeing on the frequency-hopping pattern.
- **Pairing (Security):** To ensure that your neighbor cannot accidentally connect to your speakers, devices usually require a pairing process. They generate and exchange a secure digital key (often verified by typing a PIN or confirming a prompt). Once paired, the devices remember this key and can connect automatically in the future using encrypted signals, keeping the data safe from eavesdroppers.

### Bluetooth Variant
As Bluetooth evolved, a major new variant was introduced to address a critical limitation: power consumption.
#### Bluetooth Classic
The original Bluetooth, often called Bluetooth Classic or BR/EDR (Basic Rate / Enhanced Data Rate), is designed for continuous data streaming. It provides data rates up to 3 Mbps and is ideal for applications like audio streaming (headphones, speakers) or fast file transfers. However, it consumes a relatively large amount of power, which is a problem for small battery-powered devices like fitness bands or smart sensors that need to run for months on a coin cell battery.
#### Bluetooth Low Energy (BLE)
Bluetooth Low Energy (BLE), introduced in Bluetooth 4.0 (2010), was designed from the ground up with energy efficiency as the top priority. BLE achieves dramatically lower power consumption through several design choices:
- **Short transmission bursts**: instead of maintaining a continuous connection, BLE devices sleep most of the time and wake up only briefly to send or receive small packets of data.
- **Fewer channels**: BLE uses only 40 channels (each 2 MHz wide), of which 3 are dedicated advertising channels used for discovery.
- **Simpler connection process**: BLE connections are faster to establish and terminate.
- **Optimized packet structure**: BLE packets are smaller and simpler.

A BLE device can run for months or even years on a small coin cell battery, making it ideal for:

- Fitness trackers and smartwatches
- Medical sensors (heart rate monitors, glucose meters)
- Smart home devices (locks, lights, thermostats)
- Beacon technology (location services in stores)
- Industrial IoT sensors

### The Bluetooth Protocol Stack
Bluetooth is not a single protocol but a stack of protocols, organized in layers. Each layer handles a specific part of the communication, from physical radio transmission all the way up to the application that the user interacts with.

<img src="./attachments/bluetooth_stack.png" height="400px"/>

The Bluetooth protocol stack is divided into two main parts: the host stack and the controller stack
### Controller Stack
The Controller Stack manages hardware-level operations and low-level link control. It includes:
#### The Physical Layer (Radio)
The bottom of the stack is the radio layer. This is the actual hardware: the antenna that transmits and receives radio waves, and the circuits that perform modulation and demodulation. This layer handles the raw transmission of bits over the air using GFSK modulation and FHSS.
#### Baseband Layer
Just above the physical radio is the baseband layer. This layer manages the timing and structure of how bits are grouped and transmitted. It handles:
- **Packet formation**: data is grouped into packets (small bundles) with headers containing addressing and control information.
- **Error detection**: techniques like CRC (Cyclic Redundancy Check) are used to detect transmission errors.
- **Synchronization**: the master's clock is used to keep all devices in the piconet synchronized.
- **Link types**: Bluetooth supports two types of links:
    - **SCO (Synchronous Connection-Oriented)**: for real-time audio, where some data loss is acceptable but timing must be precise used for phone calls.
    - **ACL (Asynchronous Connection-Less)**: for general data transfer, where no data can be lost used for file transfers, sensors.
#### LMP: Link Manager Protocol
The Link Manager Protocol (LMP) is responsible for setting up and managing Bluetooth links. It handles:
- Pairing and authentication: verifying that both devices are who they claim to be.
- Encryption: scrambling data so it cannot be read by unauthorized listeners.
- Power management: negotiating low-power sleep states to save battery.
- Role switching: a device can request to switch from slave to master if needed.

### Host Controller Interface
The HCI (Host Controller Interface) is a standardized boundary between the Bluetooth hardware (radio + baseband + LMP) and the software running on the host processor usually a microcontroller or CPU. It defines a set of commands the host can send to the controller and events the controller sends back.

This separation means that the Bluetooth radio can be a separate chip communicating with the main processor over a simple interface like UART or USB.

### Host Stack
#### L2CAP: Logical Link Control and Adaptation Protocol
L2CAP is a layer in Bluetooth that helps different applications share the same Bluetooth connection. It works like a middle layer between the application and the lower Bluetooth system.

Its main job is multiplexing, which means it allows multiple data streams to use one connection at the same time without mixing them up. Each application gets its own logical channel.

L2CAP also handles packet splitting and combining. If data is too large, it breaks it into smaller pieces before sending. At the receiver side, it puts the pieces back together so the original data is restored correctly.

It also supports quality of service (QoS), which helps decide how data should be handled based on the application. For example, audio needs fast delivery, while file transfer needs more accuracy.

#### Upper-Layer Bluetooth Protocols
Above the L2CAP layer, Bluetooth uses several upper-layer protocols that provide different communication services such as serial communication, file transfer, internet access, audio streaming, and telephony. These protocols allow Bluetooth devices to support many types of applications and services.

**RFCOMM** is a Bluetooth protocol that acts like a simple serial cable replacement. It imitates traditional serial (RS-232) communication, so older applications designed for wired serial ports can easily work over Bluetooth. It runs on top of L2CAP and is widely used in devices like Bluetooth modules in microcontroller projects, GPS devices, and simple wireless sensors. It is also based on the ETSI TS 07.10 standard, which defines how serial communication is handled.

**OBEX (Object Exchange)**  is used for transferring objects such as files, images, and contacts between devices. It is commonly found in file sharing and contact synchronization features. 

**WAP (Wireless Application Protocol)** was used in early mobile devices to access basic internet services over Bluetooth and other wireless networks, although it is now mostly outdated. 

**TCS (Telephony Control Protocol)** is used for managing voice communication services, such as setting up and ending calls, and is mainly used in cordless phone systems and voice gateways.

Other protocols at this level include:
- **BNEP** (Bluetooth Network Encapsulation Protocol): for networking over Bluetooth.
- **AVDTP** (Audio/Video Distribution Transport Protocol): for streaming audio.
- **AVCTP**: for audio/video remote control.

### Profiles layer
At the top of the stack are profiles. A profile is a specification that defines exactly which protocols and features a device must support to perform a particular function. Profiles ensure interoperability: if two devices both implement the same profile, they are guaranteed to work together.

Some important Bluetooth profiles include:

| Profile                             | Abbreviation | Use                                                    |
| ----------------------------------- | ------------ | ------------------------------------------------------ |
| Serial Port Profile                 | SPP          | Wireless serial connection                             |
| Hands-Free Profile                  | HFP          | Phone calls through car audio or headsets              |
| Advanced Audio Distribution Profile | A2DP         | High-quality stereo audio streaming                    |
| Audio/Video Remote Control Profile  | AVRCP        | Media playback control (play, pause, skip)             |
| Human Interface Device Profile      | HID          | Keyboards, mice, game controllers                      |
| File Transfer Profile               | FTP          | Sending files between devices                          |
| Health Device Profile               | HDP          | Medical sensor data (blood pressure, glucose monitors) |


### BLE Architecture
Bluetooth Low Energy (BLE) introduces several improvements and modifications over Bluetooth Classic, particularly in the Host Layer, by defining its own lightweight and power-efficient protocol structure. BLE is specifically designed for low-power wireless communication used in IoT devices such as sensors, smart switches, wearables, and medical devices.

The BLE architecture is divided into multiple layers, where each layer is responsible for specific communication functions. The Host Layer includes important protocols such as:

- **SMP (Security Manager Protocol)** for security and pairing
- **GAP (Generic Access Profile)** for device discovery and connection management
- **ATT/GATT (Attribute Protocol / Generic Attribute Profile)** for data organization and exchange
- **Service discovery mechanisms** for identifying available services and characteristics

<img src="./attachmentsble_archetecture.png" height="400px"/>

