#include "SE_WIFI.hpp"
#include "SE_Bluetooth.hpp"
#include <ArduinoJson.h>
#include <Preferences.h>

/* -------- Blynk externs (defined in main.cpp) -------- */
extern Preferences prefs;

extern char blynkToken[40];
extern char templateID[32];
extern char templateName[40];

extern bool blynkConfigured;
extern void startBlynk();


char ipStr[16];
WebServer server(80);

/* ========================= WEB PAGE ========================= */
void handleRoot() {

String page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>SmartElex Automation</title>

<style>
:root{
  --bg:#020617; --border:#1e293b; --accent:#0ea5e9;
  --text:#e5e7eb; --muted:#94a3b8;
  --on:#22c55e; --off:#ef4444;
}

*{box-sizing:border-box}
body{
  margin:0;background:var(--bg);
  font-family:Segoe UI,Roboto,Arial;color:var(--text);
}

/* HEADER */
.topbar{
  display:grid;
  grid-template-columns:auto 1fr auto;
  grid-template-rows:auto auto;
  column-gap:20px;
  row-gap:2px;
  padding:10px 20px;border-bottom:0.5px solid var(--border);
}
.logo img{width:220px;height:110px;object-fit:contain}
.title-main{
  grid-column:2;
  grid-row:1;
  text-align:center;
  font-size:36px;
  font-weight:600;
}
.title-sub{
  grid-column:2;
  grid-row:2;
  text-align:center;
  font-size:16px;
  color:var(--muted);
}

.datetime{text-align:right;font-size:12px;color:var(--muted)}
#wifiBadge{display:flex;align-items:center;gap:6px}
#wifiBadge .dot{width:8px;height:8px;border-radius:50%;background:var(--off)}
#wifiBadge.connected .dot{background:var(--on)}

/* TABS */
.tabs{display:flex;gap:10px;padding:12px 20px;border-bottom:1px solid var(--border)}
.tab{
  padding:10px 18px;border-radius:8px;border:1px solid var(--border);
  background:#020617;color:var(--muted);cursor:pointer;font-weight:600;
  transition:
    background 0.25s ease,
    color 0.25s ease,
    box-shadow 0.25s ease,
    transform 0.25s ease;
}

.tab.active{
  background:var(--accent);   /* Active tab highlight */
  color:#020617;              /* Dark text for contrast */
  box-shadow:0 0 10px rgba(14,165,233,.5); /* Glow effect */
}


/* MINI SWITCH */
.tab-wrap{display:flex;align-items:center;gap:10px}
.tab-switch{width:34px;height:18px;position:relative}
.tab-switch input{display:none}
.tab-slider{
  position:absolute;inset:0;background:var(--off);
  border-radius:18px;cursor:pointer;transition:.25s
}
.tab-slider:before{
  content:"";width:14px;height:14px;
  background:#fff;position:absolute;top:2px;left:2px;
  border-radius:50%;transition:.25s
}
.tab-switch input:checked + .tab-slider{background:var(--on)}
.tab-switch input:checked + .tab-slider:before{transform:translateX(16px)}

/* 🔵 Hover glow */
.tab:hover{
  color: var(--text);
  box-shadow: 0 0 8px rgba(14,165,233,0.35);
  transform: translateY(-1px);
}


/* 🟢 Active tab glow */
.tab.active{
  background: var(--accent);
  color:#020617;

  box-shadow:
    0 0 10px rgba(14,165,233,0.55),
    0 0 20px rgba(14,165,233,0.25);
}

/* GRID */
.grid{padding:28px;display:grid;grid-template-columns:repeat(3,1fr);gap:22px}
.card{border:1px solid var(--border);border-radius:16px;padding:22px}

.rs485-identity{
  text-align:center;
}

.rs485-identity h2{
  margin:0;
  font-size:26px;
  font-weight:600;
}

.subtitle{
  margin-top:6px;
  color:var(--muted);
  font-size:15px;
}

.desc{
  font-size:15px;
  line-height:1.7;
  color:var(--text);
}

.features{
  padding-left:18px;
  line-height:1.8;
  font-size:14.5px;
}

.features li{
  color:var(--text);
}


.cloud-card {
  max-width: 420px;
  margin: auto;
}

.cloud-card label {
  display: block;
  margin-top: 12px;
  font-size: 13px;
  color: var(--muted);
}

.cloud-card input {
  width: 100%;
  padding: 10px;
  margin-top: 4px;
  background: #020617;
  border: 1px solid var(--border);
  border-radius: 6px;
  color: var(--text);
}

.primary-btn {
  margin-top: 16px;
  width: 100%;
  padding: 12px;
  background: var(--accent);
  border: none;
  border-radius: 8px;
  font-weight: 600;
  color: #020617;
  cursor: pointer;
}

.primary-btn:hover {
  opacity: 0.9;
}

.cloud-info {
  font-size: 13px;
  color: var(--muted);
  margin-bottom: 10px;
}

.cloud-status {
  margin-top: 10px;
  font-size: 13px;
  color: #22c55e;
}

.cloud-help {
  font-size: 13px;
  color: var(--muted);
}

.cloud-help ul {
  padding-left: 18px;
  margin: 8px 0;
}

.cloud-help li {
  margin-bottom: 4px;
}

.note {
  margin-top: 6px;
  font-size: 12px;
  color: #94a3b8;
}


.subtabs {
  display: flex;
  gap: 6px;
  margin-bottom: 12px;
}

.subtab {
  flex: 1;
  padding: 8px;
  background: #020617;
  border: 1px solid var(--border);
  color: var(--muted);
  border-radius: 6px;
  cursor: pointer;
  font-size: 13px;
}

.subtab.active {
  background: var(--accent);
  color: #020617;
  font-weight: 600;
}

.cloud-pane {
  display: none;
}

.cloud-pane.active {
  display: block;
}


/* ===== LED INDICATOR (FIXED) ===== */
.led{
  width:14px;height:14px;border-radius:50%;
  background:var(--off);
  display:inline-block;
  margin-left:8px;
  box-shadow:0 0 6px rgba(239,68,68,0.6);
}
.led.on{
  background:var(--on);
  box-shadow:0 0 8px rgba(34,197,94,0.8);
}

.btn{width:100%;padding:12px;border-radius:10px;border:1px solid var(--border);background:#020617;color:#fff}
.btn:disabled{opacity:.4}
.btn:hover{
  background:var(--accent);
  color:#020617;
  box-shadow: 0 0 12px rgba(14,165,233,0.45);
}

.tab-content{display:none}
.tab-content.active{display:block}

.footer{text-align:center;padding:14px;font-size:12px;color:var(--muted)}
</style>

<script>
let activeControl = "WEB";

function openTab(id){
  document.querySelectorAll('.tab').forEach(t=>t.classList.remove('active'));
  document.querySelectorAll('.tab-content').forEach(c=>c.classList.remove('active'));
  document.getElementById(id+'-tab').classList.add('active');
  document.getElementById(id).classList.add('active');
}

/* ===== ONLY ONE SWITCH ON AT A TIME ===== */
function exclusiveSwitch(activeCheckbox){
  document.querySelectorAll(".tab-switch input").forEach(sw=>{
    if(sw !== activeCheckbox){
      sw.checked = false;
    }
  });
}

function relay(ch){
  if(activeControl!=="WEB") return;
  fetch('/Switch'+ch);
}

function setControl(src, enabled){
  activeControl = enabled ? src : "NONE";
  document.querySelectorAll(".btn").forEach(b=>{
    b.disabled = (activeControl!=="WEB");
  });
}

function updateDateTime(){
  const now = new Date();
  document.getElementById("date").innerText = now.toLocaleDateString();
  document.getElementById("time").innerText =
    now.toLocaleTimeString([], {hour:'2-digit', minute:'2-digit'});
}


function mirrorBLEStatus(d){
  for(let i=1;i<=6;i++){
    document.getElementById('s_ble_'+i).innerText = d[i-1] ? "ON" : "OFF";
    document.getElementById('l_ble_'+i).className = d[i-1] ? "led on" : "led";
  }
}

function refresh(){
  fetch('/getData')
    .then(r => r.json())
    .then(d => {

      // ===== Relay Status (Web Monitoring) =====
      for(let i = 1; i <= 6; i++){
        document.getElementById('s' + i).innerText =
          d[i - 1] ? "ON" : "OFF";

        document.getElementById('l' + i).className =
          d[i - 1] ? "led on" : "led";
      }

      // ===== Wi-Fi Status =====
      d[6]
        ? wifiBadge.classList.add("connected")
        : wifiBadge.classList.remove("connected");

      mirrorBLEStatus(d);

    });
}


setInterval(refresh,400);
setInterval(updateDateTime,1000);
updateDateTime();
</script>
</head>

<body>

<div class="topbar">
  <div class="logo"><img src="https://smartelex.robu.in/wp-content/uploads/2024/01/MicrosoftTeams-image-3.png"></div>
  <div>
    <div class="title-main">SmartElex Industrial Programmable Controller</div>
    <div class="title-sub">Industrial 6-Channel Relay Control Module – ESP32-S3</div>
  </div>
  <div>
    <div class="datetime"><div id="date">--</div><div id="time">--</div></div>
    <div class="wifi-badge" id="wifiBadge"><span class="dot"></span>Wi-Fi</div>
  </div>
</div>

<div class="tabs">

  <div id="web-tab" class="tab active" onclick="openTab('web')">
    <div class="tab-wrap">
      Web Page Control
      <label class="tab-switch" onclick="event.stopPropagation()">
        <input type="checkbox" checked
               onchange="exclusiveSwitch(this); setControl('WEB',this.checked)">
        <span class="tab-slider"></span>
      </label>
    </div>
  </div>

<div id="ble-tab" class="tab" onclick="openTab('ble')">
  <div class="tab-wrap">
    BLE Control
  </div>
</div>

  <div id="rs485-tab" class="tab" onclick="openTab('rs485')">
    <div class="tab-wrap">
      RS485 Interface
    </div>
  </div>

  <div id="cloud-tab" class="tab" onclick="openTab('cloud')">
    <div class="tab-wrap">
      Cloud Control
    </div>
  </div>

</div>

<div id="web" class="tab-content active">
  <div class="grid">
    <script>
      for(let i=1;i<=6;i++)
        document.write(`<div class="card">
          <h3>CH${i} <span id="l${i}" class="led"></span></h3>
          <button class="btn" onclick="relay(${i})">TOGGLE</button>
          Status: <span id="s${i}">--</span>
        </div>`);
    </script>
  </div>
</div>

<div id="ble" class="tab-content">
  <div class="grid">

    <!-- BLE Overview -->
    <div class="card">
      <h3>Bluetooth Low Energy (BLE)</h3>
      <p>
        Local wireless interface for commissioning, diagnostics,
        and direct relay control without internet dependency.
      </p>
      <p style="color:var(--muted);font-size:13px;">
        Recommended for on-site setup and maintenance.
      </p>
    </div>
    <!-- Device Info -->
    <div class="card">
      <h3>BLE Device</h3>
      <table style="width:100%;font-size:14px;border-collapse:collapse;">
        <tr>
          <td style="color:var(--muted);padding:4px 0;">Device Name</td>
          <td><b>ESP32S3-Relay-6CH</b></td>
        </tr>
        <tr>
          <td style="color:var(--muted);padding:4px 0;">Protocol</td>
          <td>BLE UART / Notify</td>
        </tr>
        <tr>
          <td style="color:var(--muted);padding:4px 0;">Usage</td>
          <td>Local Control</td>
        </tr>
      </table>
    </div>

    <!-- Supported Commands -->
    <div class="card">
      <h3>Supported Commands</h3>
      <ul style="margin:0;padding-left:18px;line-height:1.8;font-size:14px;">
        <li><b>1 – 6</b> → Toggle Relay Channels</li>
        <li><b>7</b> → Turn <b>ALL Relays ON</b></li>
        <li><b>8</b> → Turn <b>ALL Relays OFF</b></li>
      </ul>
      <p style="color:var(--muted);font-size:12px;margin-top:8px;">
        Commands are sent as single ASCII characters.
      </p>
    </div>

  </div>
</div>




<div id="rs485" class="tab-content">
  <div class="grid">

    <!-- RS485 Overview -->
    <div class="card">
      <h3>RS485 Interface</h3>
      <p>
        Wired industrial communication interface for reliable,
        noise-immune relay control and automation system integration.
      </p>
      <p style="color:var(--muted);font-size:13px;">
        Recommended for factory floor, control panels, and long-distance communication.
      </p>
    </div>

    <!-- Communication Parameters -->
    <div class="card">
      <h3>Communication Parameters</h3>
      <table style="width:100%;font-size:14px;border-collapse:collapse;">
        <tr>
          <td style="color:var(--muted);padding:4px 0;">Signal Lines</td>
          <td>A, B, GND</td>
        </tr>
        <tr>
          <td style="color:var(--muted);padding:4px 0;">Baud Rate</td>
          <td>9600 bps</td>
        </tr>
        <tr>
          <td style="color:var(--muted);padding:4px 0;">Frame Format</td>
          <td>8-N-1</td>
        </tr>
      </table>
    </div>

    <!-- Supported Commands -->
    <div class="card">
      <h3>Supported Commands</h3>
      <ul style="margin:0;padding-left:18px;line-height:1.8;font-size:14px;">
        <li><b>1 – 6</b> → Toggle Individual Relay Channels</li>
        <li><b>7</b> → Turn <b>ALL Relays ON</b></li>
        <li><b>8</b> → Turn <b>ALL Relays OFF</b></li>
      </ul>
      <p style="color:var(--muted);font-size:12px;margin-top:8px;">
        Commands are transmitted as single ASCII characters over RS485.
      </p>
    </div>

  </div>
</div>

<div id="cloud" class="tab-content">
  <div class="grid cloud-grid">

    <!-- ===== CLOUD SETUP CARD ===== -->
    <div class="card cloud-card">
      <h3>☁ Cloud Setup (Blynk)</h3>

      <p class="cloud-info">
        Connect this device to Blynk Cloud for remote relay control.
      </p>

      <label>Template ID</label>
      <input id="tmpl" placeholder="TMPLxxxxxxx">

      <label>Template Name</label>
      <input id="name" placeholder="SmartElex Relay 6CH">

      <label>Auth Token</label>
      <input id="token" placeholder="Paste Blynk Auth Token">

      <button class="primary-btn" onclick="saveBlynk()">
        Connect
      </button>

      <p id="status" class="cloud-status"></p>
    </div>

    <!-- ===== CLOUD HELP CARD ===== -->
    <div class="card cloud-card">
      <h3>📘 Blynk Configuration</h3>

      <h4>🔌 Virtual Pin Mapping</h4>
      <ul>
        <li><b>V1 – V6</b> : Relay 1 – Relay 6</li>
        <li><b>V7</b> : ALL ON / ALL OFF</li>
      </ul>

      <h4>📱 Widget Setup</h4>
      <ul>
        <li>Widget: <b>Button</b></li>
        <li>Mode: <b>Switch</b></li>
        <li>ON = 1, OFF = 0</li>
      </ul>

      <p class="note">
        Supports both <b>Blynk Mobile App</b> and
        <b>Blynk Web Dashboard</b>.
      </p>
    </div>

  </div>
</div>


<script>
function openCloudTab(tab) {
  document.querySelectorAll(".cloud-pane").forEach(p => p.classList.remove("active"));
  document.querySelectorAll(".subtab").forEach(b => b.classList.remove("active"));

  document.getElementById("cloud-" + tab).classList.add("active");
  event.target.classList.add("active");
}
</script>



<script>
function saveBlynk() {
  document.getElementById("status").innerText = "Connecting to Blynk...";

  fetch("/blynk", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({
      template: document.getElementById("tmpl").value,
      name: document.getElementById("name").value,
      token: document.getElementById("token").value
    })
  })
  .then(res => res.text())
  .then(msg => {
    document.getElementById("status").innerText = msg;
  })
  .catch(() => {
    document.getElementById("status").innerText = "Connection failed";
  });
}
</script>

<div class="footer">© SmartElex Automation • Industrial IoT</div>
</body>
</html>
)rawliteral";

server.send(200,"text/html",page);
}



/* ========================= API ========================= */
void handleGetData() {
  String json = "[";

  for (int i = 0; i < 6; i++) {
    json += Relay_Flag[i] ? "1" : "0";
    json += ",";
  }

  bool staConnected = (WiFi.status() == WL_CONNECTED);
  bool apRunning    = (WiFi.getMode() & WIFI_AP);

  json += staConnected ? "1" : "0";
  json += ",";
  json += deviceConnected ? "1" : "0";
  
  json += "]";

  server.send(200, "application/json", json);
}


void handleSwitch(uint8_t ledNumber) {
  uint8_t Data[1]={0};
  Data[0]=ledNumber+48;
  Relay_Analysis(Data,WIFI_Mode);
  server.send(200, "text/plain", "OK");
}
void handleSwitch1() { handleSwitch(1); }
void handleSwitch2() { handleSwitch(2); }
void handleSwitch3() { handleSwitch(3); }
void handleSwitch4() { handleSwitch(4); }
void handleSwitch5() { handleSwitch(5); }
void handleSwitch6() { handleSwitch(6); }
void handleSwitch7() { handleSwitch(7); }
void handleSwitch8() { handleSwitch(8); }

/* ========================= WIFI ========================= */
void WIFI_Init()
{
  /* ---------- WIFI MODE ---------- */
  WiFi.mode(WIFI_AP_STA);
  WiFi.setHostname(HOSTNAME);

  WiFiManager wm;
  wm.setCaptivePortalEnable(true);
  wm.setConfigPortalBlocking(true);
  wm.setConfigPortalTimeout(180);

  bool res = wm.autoConnect(APSSID, APPSK);
  if (!res) {
    ESP.restart();
  }

  /* ---------- STA CONNECTED ---------- */
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);

  /* ---------- mDNS ---------- */
  MDNS.begin(HOSTNAME);

  /* ---------- WEB ROUTES ---------- */
  server.on("/", handleRoot);
  server.on("/getData", handleGetData);

  server.on("/Switch1", handleSwitch1);
  server.on("/Switch2", handleSwitch2);
  server.on("/Switch3", handleSwitch3);
  server.on("/Switch4", handleSwitch4);
  server.on("/Switch5", handleSwitch5);
  server.on("/Switch6", handleSwitch6);

  server.on("/AllOn",  handleSwitch7);
  server.on("/AllOff", handleSwitch8);

  /* ---------- CLOUD (BLYNK) CONFIG ---------- */
  server.on("/blynk", HTTP_POST, []() {

    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, server.arg("plain"))) {
      server.send(400, "text/plain", "Invalid JSON");
      return;
    }

    const char* token = doc["token"];
    const char* tmpl  = doc["template"];
    const char* name  = doc["name"];

    if (!token || strlen(token) < 10) {
      server.send(400, "text/plain", "Invalid Auth Token");
      return;
    }

    /* Save to NVS */
    prefs.begin("blynk", false);
    prefs.putString("token", token);
    prefs.putString("tmpl", tmpl ? tmpl : "");
    prefs.putString("name", name ? name : "");
    prefs.end();

    /* Copy to runtime */
    strncpy(blynkToken, token, sizeof(blynkToken));
    if (tmpl) strncpy(templateID, tmpl, sizeof(templateID));
    if (name) strncpy(templateName, name, sizeof(templateName));

    blynkConfigured = true;

    server.send(200, "text/plain", "Blynk Config Saved. Connecting...");

    /* 🔥 Start Blynk immediately */
    startBlynk();
  });

  server.begin();

  /* ---------- STATUS LED ---------- */
  RGB_Light(0, 60, 0);
  delay(1000);
  RGB_Light(0, 0, 0);
}


void WIFI_Loop(){
  server.handleClient();
}
