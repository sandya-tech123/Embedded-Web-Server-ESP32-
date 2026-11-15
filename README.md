# ESP32 Embedded Web Server

A software-only project: an ESP32 runs an HTTP server that serves a live dashboard and a REST API.
No extra hardware is needed to try it, because a browser simulator runs the same dashboard with a mocked API.

## Structure
```
firmware/            ESP32 code (PlatformIO / Arduino)
  src/main.cpp         web server + REST API
  src/config.h         Wi-Fi credentials, LED pin
  data/index.html      dashboard served from LittleFS
simulator/           run in any browser, no hardware
  mock.js              fake ESP32 API (replaces fetch)
  build.py             builds simulator/index.html
  index.html           ready to open / host on GitHub Pages
docs/
  API.md, ARCHITECTURE.md, REPORT.md
```

## Run it without hardware
Open `simulator/index.html` in a browser (or host it with GitHub Pages: Settings -> Pages -> branch `main`, folder `/simulator`).

## Run on a real ESP32
1. Install [PlatformIO](https://platformio.org/).
2. Edit `firmware/src/config.h` with your Wi-Fi name and password.
3. `cd firmware && pio run -t upload` (firmware) then `pio run -t uploadfs` (dashboard).
4. Open the serial monitor (115200); it prints the IP. Browse to `http://<ip>/`.
   If Wi-Fi fails, the board starts its own access point `ESP32-WebServer` (password `12345678`) at `http://192.168.4.1`.

## API summary
| Method | Path | Description |
|---|---|---|
| GET | /api/status | uptime, heap, RSSI, temperature, LED |
| GET | /api/sensor | temperature, humidity |
| GET | /api/wifi | ssid, ip, rssi |
| POST | /api/led | `{"on":true}` / `{"on":false}` |

Note: firmware is written against the ESP32 Arduino core and has not been compiled on hardware in this build environment; the simulator was tested in a headless browser.

## License
MIT
