#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>
 
#define RST_PIN 22
#define SS_PIN 5
#define BUZZER_PIN 14
 
MFRC522 rfid(SS_PIN, RST_PIN);
 
const char* ssid = "sycek";              
const char* password = "Kks3a8kmfhGf";      
const String firebaseHost = "https://employee-database-78acd-default-rtdb.europe-west1.firebasedatabase.app";
 
const int lateHour = 8;
const int lateMinute = 15;
 
void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
 
  SPI.begin(18, 19, 23, SS_PIN);
  rfid.PCD_Init();
 
  WiFi.begin(ssid, password);
  Serial.print("📡 Připojuji se k WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi připojeno");
 
  // Konfigurace času (Evropa, Praha - GMT+2 v létě)
  configTime(7200, 0, "pool.ntp.org", "time.nist.gov"); // 7200 = +2h, 3600 = DST
  delay(2000); // Počkáme na čas
}
 
void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;
 
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  Serial.println("🔍 Detekován RFID čip: " + uid);
 
  // Získání času
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("❌ Nelze načíst čas.");
    delay(3000);
    return;
  }
 
  char timeStr[6];
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
  bool isLate = (timeinfo.tm_hour > lateHour || (timeinfo.tm_hour == lateHour && timeinfo.tm_min > lateMinute));
 
  Serial.printf("🕒 Aktuální čas: %s\n", timeStr);
 
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = firebaseHost + "/employees/" + uid + ".json";
    http.begin(url);
    int httpCode = http.GET();
 
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("📥 Odpověď Firebase: " + payload);
 
      if (payload.indexOf("access") > 0) {
        String accessStr = getValue(payload, "\"access\":", ",");
        String name = getValue(payload, "\"name\":\"", "\"");
        bool access = (accessStr == "true");
 
        Serial.println("👤 Jméno: " + name);
 
        if (access) {
          if (isLate) {
            Serial.println("❌ Přístup odepřen – pozdní příchod");
            beepBuzzer();
            beepBuzzer();
          } else {
            Serial.println("✅ Přístup povolen – včas");
           
          }
        } else {
          Serial.println("⛔️ Přístup odepřen – zakázáno v databázi");
        }
 
        // Log do "logs" sekce pro zobrazení na webu
        String logUrl = firebaseHost + "/logs.json";
        String logData = "{\"name\":\"" + name + "\",\"arrival_time\":\"" + timeStr + "\",\"late\":" + (isLate ? "true" : "false") + "}";
        http.begin(logUrl);
        http.addHeader("Content-Type", "application/json");
        int postCode = http.POST(logData);
        if (postCode > 0) {
          Serial.println("📤 Záznam odeslán do databáze.");
        } else {
          Serial.println("❌ Chyba při odesílání logu.");
        }
 
      } else {
        Serial.println("❓ UID není v databázi.");
      }
 
    } else {
      Serial.println("❌ Chyba připojení k Firebase. Kód: " + String(httpCode));
    }
 
    http.end();
  }
 
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(3000);
}
 
String getValue(String data, String key, String delimiter) {
  int startIndex = data.indexOf(key);
  if (startIndex == -1) return "";
  startIndex += key.length();
  int endIndex = data.indexOf(delimiter, startIndex);
  return data.substring(startIndex, endIndex);
}
 
void beepBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);
}