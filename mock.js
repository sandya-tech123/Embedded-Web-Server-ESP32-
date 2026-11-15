// Software-only ESP32 simulator: replaces window.fetch for /api/* so the real
// dashboard (firmware/data/index.html) runs in any browser with no hardware.
(function () {
  const t0 = Date.now();
  let led = false;
  const json = (o, status = 200) =>
    Promise.resolve(new Response(JSON.stringify(o), { status, headers: { 'Content-Type': 'application/json' } }));
  const temp = () => 26 + 3 * Math.sin((Date.now() - t0) / 20000) + (Math.random() - 0.5) * 0.2;
  const hum = () => 55 + 10 * Math.cos((Date.now() - t0) / 30000);
  const realFetch = window.fetch.bind(window);

  window.fetch = function (url, opts = {}) {
    const method = (opts.method || 'GET').toUpperCase();
    if (url === '/api/status' && method === 'GET')
      return json({ uptime_s: Math.floor((Date.now() - t0) / 1000), free_heap: 214000 - Math.floor(Math.random() * 3000),
                    rssi: -58 - Math.floor(Math.random() * 6), temperature: temp(), led });
    if (url === '/api/sensor' && method === 'GET') return json({ temperature: temp(), humidity: hum() });
    if (url === '/api/wifi' && method === 'GET') return json({ ssid: 'SimNet', ip: '192.168.4.1', rssi: -60 });
    if (url === '/api/led' && method === 'POST') {
      try {
        const b = JSON.parse(opts.body);
        if (typeof b.on !== 'boolean') throw 0;
        led = b.on;
        return json({ led });
      } catch (e) { return json({ error: 'body must be {"on":true|false}' }, 400); }
    }
    if (String(url).startsWith('/api/')) return json({ error: 'not found' }, 404);
    return realFetch(url, opts);
  };
})();
