# Architecture

```
Browser (dashboard, polls every 2 s)
   |  HTTP :80
   v
ESP32  WebServer
   |-- static files  <- LittleFS (index.html)
   |-- /api/status, /api/sensor, /api/wifi (GET)
   '-- /api/led (POST) -> GPIO2
Wi-Fi: STA mode, falls back to SoftAP after 15 s
```

- **Firmware**: Arduino framework, `WebServer.h` (synchronous, single core loop), ArduinoJson 7.
- **Frontend**: one static HTML file, vanilla JS, canvas chart, light/dark theme.
- **Simulator**: `mock.js` overrides `window.fetch` for `/api/*`, so the identical frontend runs in a browser.
- **Sensor**: simulated sine waves; swap `readTemperature()`/`readHumidity()` for a DHT22/BME280 driver.
