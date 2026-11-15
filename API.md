# REST API

All responses are JSON with `Access-Control-Allow-Origin: *`.

## GET /api/status
```json
{"uptime_s":125,"free_heap":214000,"rssi":-60,"temperature":26.4,"led":false}
```

## GET /api/sensor
```json
{"temperature":26.4,"humidity":55.2}
```

## GET /api/wifi
```json
{"ssid":"MyWifi","ip":"192.168.1.50","rssi":-60}
```

## POST /api/led
Request: `{"on":true}`. Response `200 {"led":true}`.
Invalid body returns `400 {"error":"body must be {\"on\":true|false}"}`.

Example: `curl -X POST http://<ip>/api/led -d '{"on":true}'`
