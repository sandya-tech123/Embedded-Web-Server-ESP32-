"""Builds simulator/index.html: the real dashboard + mock ESP32 API in one file."""
import pathlib
root = pathlib.Path(__file__).resolve().parent.parent
page = (root / "firmware/data/index.html").read_text()
mock = (root / "simulator/mock.js").read_text()
banner = '<p class="sub" style="color:#b54708">Simulation mode: no hardware, API is mocked in the browser.</p>'
page = page.replace("<script>", "<script>\n" + mock + "\n</script>\n<script>", 1)
page = page.replace('<p class="sub" id="conn">', banner + '\n  <p class="sub" id="conn">', 1)
(root / "simulator/index.html").write_text(page)
print("wrote simulator/index.html", len(page), "bytes")
