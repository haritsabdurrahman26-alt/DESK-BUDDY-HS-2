/*
   DESK BUDDY ULTRA VERSION
   BY : HS PROJECT
   Wemos D1 Mini + OLED 1.3 Inch
   Dasai Mochi Style + Living Animation

   FITUR:
   - Mata kotak ala Dasai Mochi
   - Random blink natural
   - Pupil bergerak kiri/kanan
   - Happy / Focus / Tired / Sleep mode
   - Sleep animation + Zzz
   - Jam set via Web
   - WiFi AP Mode
   - Motivasi otomatis
   - Desk buddy terasa hidup

   WiFi:
   SSID     : DeskBuddy
   Password : 12345678

   Web:
   http://192.168.4.1
*/

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);

ESP8266WebServer server(80);

// ======================
// WIFI
// ======================
const char* ssid = "DeskBuddy";
const char* password = "12345678";

// ======================
// CLOCK
// ======================
int jam = 8;
int menit = 0;
unsigned long lastClock = 0;

// ======================
// ANIMATION
// ======================
bool blinkState = false;
unsigned long lastBlink = 0;
unsigned long nextBlink = 3000;

int pupilOffset = 0;
int pupilDirection = 1;
unsigned long lastEyeMove = 0;

// ======================
// MOTIVASI
// ======================
String motivasi[] = {
  "Tetap Semangat!",
  "Hari Ini Menang!",
  "Fokus = Hasil",
  "Belajar Lagi Yuk",
  "Jangan Menyerah!"
};

int motivasiIndex = 0;
unsigned long lastMotivasi = 0;

// ======================
// WEB PAGE
// ======================

String htmlPage() {
  String html = R"====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Desk Buddy Control</title>

<style>
*{
  margin:0;
  padding:0;
  box-sizing:border-box;
  font-family:Arial, sans-serif;
}

body{
  background:#f5f7fa;
  padding:20px;
  text-align:center;
}

.card{
  max-width:420px;
  margin:auto;
  background:white;
  padding:30px;
  border-radius:20px;
  box-shadow:0 10px 30px rgba(0,0,0,0.08);
}

h1{
  font-size:28px;
  margin-bottom:8px;
}

.subtitle{
  color:#666;
  margin-bottom:25px;
  font-size:14px;
}

.time-box{
  background:#f0f4f8;
  padding:18px;
  border-radius:16px;
  margin-bottom:25px;
}

.current-time{
  font-size:34px;
  font-weight:bold;
  margin-top:8px;
}

label{
  display:block;
  text-align:left;
  margin-bottom:8px;
  margin-top:15px;
  font-weight:600;
}

input{
  width:100%;
  height:50px;
  border:1px solid #ddd;
  border-radius:12px;
  padding:10px;
  font-size:18px;
  text-align:center;
}

button{
  width:100%;
  height:52px;
  border:none;
  border-radius:14px;
  font-size:18px;
  font-weight:bold;
  margin-top:25px;
  cursor:pointer;
  background:#111;
  color:white;
}

button:hover{
  opacity:0.92;
}

.footer{
  margin-top:20px;
  font-size:13px;
  color:#777;
}
</style>
</head>

<body>

<div class="card">

  <h1>Desk Buddy</h1>
  <div class="subtitle">
    Dasai Mochi Smart Control Panel
  </div>

  <div class="time-box">
    Current Time
    <div class="current-time">
)====";

  if (jam < 10) html += "0";
  html += String(jam);
  html += ":";

  if (menit < 10) html += "0";
  html += String(menit);

  html += R"====(
    </div>
  </div>

  <form action="/setTime">

    <label>Set Hour (0 - 23)</label>
    <input
      type="number"
      name="jam"
      min="0"
      max="23"
      required
    >

    <label>Set Minute (0 - 59)</label>
    <input
      type="number"
      name="menit"
      min="0"
      max="59"
      required
    >

    <button type="submit">
      Save Time
    </button>

  </form>

  <div class="footer">
    WiFi AP : DeskBuddy
  </div>

</div>

</body>
</html>
)====";

  return html;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleSetTime() {
  if (server.hasArg("jam")) jam = server.arg("jam").toInt();
  if (server.hasArg("menit")) menit = server.arg("menit").toInt();

  if (jam < 0) jam = 0;
  if (jam > 23) jam = 23;
  if (menit < 0) menit = 0;
  if (menit > 59) menit = 59;

  server.sendHeader("Location", "/");
  server.send(303);
}

void setupWeb() {
  server.on("/", handleRoot);
  server.on("/setTime", handleSetTime);
  server.begin();
}

// ======================
// WIFI
// ======================
void setupWiFi() {
  WiFi.softAP(ssid, password);
}

// ======================
// CLOCK
// ======================
void updateClock() {
  if (millis() - lastClock >= 60000) {
    lastClock = millis();

    menit++;
    if (menit >= 60) {
      menit = 0;
      jam++;
    }
    if (jam >= 24) jam = 0;
  }
}

// ======================
// RANDOM BLINK
// ======================
void updateBlink() {
  if (!blinkState && millis() - lastBlink >= nextBlink) {
    blinkState = true;
    lastBlink = millis();
  }

  if (blinkState && millis() - lastBlink >= 180) {
    blinkState = false;
    lastBlink = millis();
    nextBlink = random(2500, 6000); // blink natural
  }
}

// ======================
// MOVING PUPIL
// ======================
void updatePupil() {
  if (millis() - lastEyeMove >= 400) {
    lastEyeMove = millis();

    pupilOffset += pupilDirection;

    if (pupilOffset >= 3) pupilDirection = -1;
    if (pupilOffset <= -3) pupilDirection = 1;
  }
}

// ======================
// MOTIVASI
// ======================
void updateMotivasi() {
  if (millis() - lastMotivasi >= 10000) {
    lastMotivasi = millis();

    motivasiIndex++;
    if (motivasiIndex >= 5) motivasiIndex = 0;
  }
}

// ======================
// MODE
// ======================
int getMoodMode() {
  if (jam >= 6 && jam <= 10) return 0;   // Happy
  if (jam >= 11 && jam <= 16) return 1;  // Focus
  if (jam >= 17 && jam <= 20) return 2;  // Tired
  return 3;                              // Sleep
}

// ======================
// DRAW HELPERS
// ======================
void drawTime(bool top) {
  char waktu[10];
  sprintf(waktu, "%02d:%02d", jam, menit);

  u8g2.setFont(u8g2_font_6x12_tf);

  if (top)
    u8g2.drawStr(42, 10, waktu);
  else
    u8g2.drawStr(48, 54, waktu);
}

void drawMotivasi() {
  u8g2.setFont(u8g2_font_5x8_tf);
  u8g2.drawStr(8, 62, motivasi[motivasiIndex].c_str());
}

void drawCheeks() {
  u8g2.drawCircle(12, 40, 3);
  u8g2.drawCircle(116, 40, 3);
}

void drawMouthSmile() {
  u8g2.drawLine(56, 42, 72, 42);
  u8g2.drawPixel(57, 43);
  u8g2.drawPixel(71, 43);
}

void drawMouthFlat() {
  u8g2.drawLine(58, 42, 70, 42);
}

void drawEyesBox(int pupilSize) {
  if (blinkState) {
    u8g2.drawLine(24, 26, 50, 26);
    u8g2.drawLine(78, 26, 104, 26);
    return;
  }

  u8g2.drawRFrame(20, 16, 34, 20, 5);
  u8g2.drawDisc(37 + pupilOffset, 26, pupilSize);

  u8g2.drawRFrame(74, 16, 34, 20, 5);
  u8g2.drawDisc(91 + pupilOffset, 26, pupilSize);
}

// ======================
// FACE MODES
// ======================
void drawHappyMode() {
  drawEyesBox(6);
  drawMouthSmile();
}

void drawFocusMode() {
  drawEyesBox(5);
  drawMouthFlat();
}

/*
TAMBAHKAN / GANTI BAGIAN INI
untuk:
- tired mode lebih hidup
- startup animation / pembukaan lebih keren
*/


// ======================
// STARTUP ANIMATION
// ======================
void startupAnimation() {
  // loading bar + opening

  for (int i = 0; i <= 100; i += 4) {
    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(28, 16, "HS PROJECT");
    u8g2.drawStr(20, 30, "Desk Buddy Boot");

    u8g2.drawFrame(14, 42, 100, 10);
    u8g2.drawBox(16, 44, i, 6);

    u8g2.sendBuffer();
    delay(40);
  }

  delay(800);

  // wajah muncul perlahan
  for (int i = 0; i < 2; i++) {
    u8g2.clearBuffer();

    u8g2.drawRFrame(20, 16, 34, 20, 5);
    u8g2.drawDisc(37, 26, 5);

    u8g2.drawRFrame(74, 16, 34, 20, 5);
    u8g2.drawDisc(91, 26, 5);

    u8g2.drawLine(58, 42, 70, 42);

    u8g2.sendBuffer();
    delay(300);

    u8g2.clearBuffer();
    u8g2.sendBuffer();
    delay(150);
  }
}


// ======================
// TIRED MODE 
// ======================
void drawTiredMode() {
  
  int tiredOffset = (millis() / 500) % 2; // animasi pelan

  // mata kiri
  u8g2.drawRFrame(20, 16, 34, 20, 5);
  u8g2.drawBox(26, 24 + tiredOffset, 16, 4);

  // mata kanan
  u8g2.drawRFrame(74, 16, 34, 20, 5);
  u8g2.drawBox(80, 24 + tiredOffset, 16, 4);

  // mulut datar
  drawMouthFlat();

  // animasi titik capek ...
  u8g2.setFont(u8g2_font_5x8_tf);

  if ((millis() / 700) % 3 == 0) {
    u8g2.drawStr(56, 48, ".");
  }
  else if ((millis() / 700) % 3 == 1) {
    u8g2.drawStr(54, 48, "..");
  }
  else {
    u8g2.drawStr(52, 48, "...");
  }
}

void drawSleepMode() {
  u8g2.drawRFrame(20, 16, 34, 20, 5);
  u8g2.drawLine(24, 26, 50, 26);

  u8g2.drawRFrame(74, 16, 34, 20, 5);
  u8g2.drawLine(78, 26, 104, 26);

  // animated Z
  u8g2.setFont(u8g2_font_6x12_tf);
  if ((millis() / 500) % 2 == 0)
    u8g2.drawStr(95, 14, "Zz");
  else
    u8g2.drawStr(98, 12, "z");

  drawMouthFlat();
}

// ======================
// MAIN DRAW
// ======================
void drawDeskBuddy() {
  int mode = getMoodMode();

  u8g2.clearBuffer();

  // WiFi status
  u8g2.setFont(u8g2_font_5x8_tf);
  u8g2.drawStr(2, 8, "AP ON");

  switch (mode) {
    case 0:
      drawHappyMode();
      drawTime(false);
      drawMotivasi();
      break;

    case 1:
      drawFocusMode();
      drawTime(false);
      drawMotivasi();
      break;

    case 2:
      drawTiredMode();
      drawTime(false);
      drawMotivasi();
      break;

    case 3:
      drawSleepMode();
      drawTime(true);
      break;
  }

  drawCheeks();
  u8g2.sendBuffer();
}

// ======================
// SETUP
// ======================
void setup() {
  Wire.begin(D2, D1);
  u8g2.begin();

  randomSeed(analogRead(A0));

  startupAnimation();   // <-- tambahkan ini

  setupWiFi();
  setupWeb();
}
// ======================
// LOOP
// ======================
void loop() {
  server.handleClient();

  updateClock();
  updateBlink();
  updatePupil();
  updateMotivasi();

  drawDeskBuddy();
}