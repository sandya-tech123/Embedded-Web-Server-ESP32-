---
title: "Embedded Web Server using ESP32 - Project Report"
---

# 1. Abstract
This project implements an embedded web server on the ESP32 microcontroller. The device serves a responsive monitoring dashboard and a REST API for reading sensor data, device status and controlling an output (LED) over Wi-Fi. The work is software-only: a browser-based simulator reproduces the device API so the system can be demonstrated and tested without hardware.

# 2. Objectives
- Host an HTTP server on the ESP32 and serve a web UI from flash (LittleFS).
- Provide a JSON REST API for monitoring and control.
- Provide Wi-Fi station mode with automatic access-point fallback.
- Provide a hardware-free simulator, documentation and a GitHub-ready repository.

# 3. Technologies
| Layer | Choice |
|---|---|
| MCU | ESP32 (dual-core, Wi-Fi) |
| Framework | Arduino core, PlatformIO |
| Server | WebServer.h |
| Storage | LittleFS |
| Data format | JSON (ArduinoJson 7) |
| Frontend | HTML, CSS, vanilla JavaScript, canvas |

# 4. System design
The browser requests the dashboard from the ESP32 and then polls `/api/status` and `/api/sensor` every 2 seconds. A POST to `/api/led` toggles GPIO 2. On boot the ESP32 tries to join the configured Wi-Fi for 15 seconds; on failure it starts its own access point. See `ARCHITECTURE.md` and `API.md`.

# 5. Implementation
- `firmware/src/main.cpp`: route registration, JSON handlers, input validation (400 on bad body), static file serving.
- `firmware/data/index.html`: live cards, temperature history chart, request log, LED toggle, dark/light theme.
- `simulator/mock.js`: in-browser fake of the four API endpoints, including error handling.

# 6. Testing
| Test | Expected | Result |
|---|---|---|
| Simulator loads dashboard | Cards populate within 2 s | Passed (headless browser) |
| LED toggle in simulator | State flips ON/OFF | Passed |
| Invalid POST body | 400 error JSON | Passed (mock) |
| Firmware compile / flash on real ESP32 | Builds and serves | Not run in this environment; to be verified on hardware |

# 7. Limitations and future work
- Sensor values are simulated; integrate DHT22/BME280.
- No authentication or HTTPS; add basic auth or TLS before exposing outside a local network.
- Polling could be replaced with WebSockets or Server-Sent Events.
- Add OTA firmware updates and an on-device Wi-Fi provisioning page.

# 8. Conclusion
The project delivers a complete embedded web server design with firmware, dashboard, simulator and documentation, ready to flash to an ESP32 or demonstrate in a browser.
