# SchoolAccessSystem
_**Školní Přístupový Systém**
_
Tento projekt slouží jako jednoduchý přístupový systém postavený na Firebase a webových technologiích. Obsahuje autentizaci uživatelů, databázi přístupových kódů.

**🚀 Funkce**

Webové přihlášení uživatele (Firebase Auth)

Evidence a správa přístupových záznamů (Realtime Database)

Push notifikace přes OneSignal (funguje na PC, mobilu, iWatch)

Hosting přes Firebase

**🛠️ Požadavky**

Firebase projekt

Node.js + Firebase CLI

Webhosting na Firebase

Účet na OneSignal (zdarma)

Arduino IDE nebo PlatformIO

ESP8266 / ESP32

RFID modul (např. RC522)

**📦 Instalace & nasazení**
Firebase nastavení
Přihlas se do Firebase:
(v cmd řádku)

firebase login

firebase init
Vyber:

Hosting

Realtime Database

Authentication

Nasazení na web

firebase deploy
- kontent byl úspěšně nahrný na webovou stránku

🔌 ESP Mikrokontrolér

Projekt obsahuje také kód pro ESP (např. ESP8266 nebo ESP32), který slouží jako fyzický přístupový bod. Tento mikrokontrolér:

Čte RFID/NFC tagy (např. pomocí modulu RC522)

Odesílá data do Firebase Realtime Database

Zpracovává odpovědi z Firebase pro povolení/odepření přístupu

**📁 Struktura**

ESP kód je uložen ve složce esp/. Např.:

esp/
├── src/
│   ├── esp32_code.ino
│   └── firebase_config.h
├── lib/
│   └── (knihovny, např. FirebaseArduino, MFRC522 atd.)

🔧 Nahrání do ESP

Otevři main.ino v Arduino IDE nebo PlatformIO.

Uprav firebase_config.h podle svých údajů z Firebase:

#define FIREBASE_HOST "tvujprojekt.firebaseio.com"
#define FIREBASE_AUTH "tvuj_firebase_secret"
#define WIFI_SSID "nazev_wifi"
#define WIFI_PASSWORD "heslo"

Připoj ESP přes USB a nahraj kód.

Struktura web. stránky
projekt/
│
├── public/
│   ├── index.html       # hlavní stránka s tabulkou přístupů
│   ├── login.html       # přihlašovací stránka
│   ├── firebase.js      # funkce pro práci s Firebase
│   └── 404.html         # kód který se spustí při nefunkčnosti nebo nekompabilitě
│
└── firebase.json        # nastavení Firebase hostingu

**🔐 Přihlášení**
Používá se Firebase Email/Password Authentication.

V login.html je formulář pro přihlášení.

Po úspěšném přihlášení je uživatel přesměrován na index.html.

**📖 Práce s přístupovými záznamy**
V aplikaci můžeš přidávat, mazat a prohlížet přístupové záznamy (např. pomocí RFID nebo čipu).

Záznamy se ukládají do Firebase Realtime Database a zobrazují v tabulce na index.html.
