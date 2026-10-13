# LDR Cloud Fan Controller

An ESP32-based IoT project that monitors ambient light using an LDR sensor and controls a simulated fan through Blynk IoT Cloud.

## Features

- Real-time light-level monitoring
- Raw analog sensor readings
- Manual fan ON/OFF control
- Automatic fan control based on a configurable light threshold
- Blynk IoT dashboard integration
- Wokwi simulation without physical hardware

## Hardware and Tools

- ESP32 DevKit
- LDR sensor module with analog output
- LED used as a simulated fan indicator
- Blynk IoT Cloud
- Wokwi ESP32 Simulator
- Arduino framework and Blynk library

## Pin Connections

| Component | ESP32 pin |
|---|---|
| LDR VCC | 3V3 |
| LDR GND | GND |
| LDR AO | GPIO34 |
| LDR DO | Not connected |
| Simulated fan output | GPIO26 |

## Blynk Datastreams

| Virtual pin | Purpose |
|---|---|
| V0 | Light level percentage |
| V1 | Raw LDR ADC reading |
| V2 | Manual fan control |
| V3 | Automatic mode |
| V4 | Light threshold |
| V5 | Fan state indicator |

## Setup

1. Open the Wokwi project or create a new ESP32 project.
2. Add the circuit using `diagram.json`.
3. Install the Blynk library listed in `libraries.txt`.
4. Copy `secrets.h.example` to `secrets.h`.
5. Add your own Blynk Template ID, Template Name and Device Auth Token to `secrets.h`.
6. Run the simulation and check the Serial Monitor.
7. Configure the matching datastreams and dashboard widgets in Blynk.

## How It Works

The ESP32 reads the LDR's analog output and converts the reading into a percentage. The sensor values are sent to Blynk Cloud.

In manual mode, the dashboard switch controls the fan output. In automatic mode, the output is controlled by comparing the measured light level with the configured threshold.

## Simulation

Wokwi project: https://wokwi.com/projects/477766097387142145

## Security

Never commit `secrets.h` or publish your Blynk Device Auth Token. The `.gitignore` file excludes the private credentials file.

## Limitations

This project has been tested in simulation. Physical fan control requires an appropriately rated driver circuit and a suitable external power supply. Do not connect a real fan directly to an ESP32 GPIO pin.

## License

This project is available under the MIT License. See `LICENSE` for details.
