#include <SD.h>
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#include <ht1632c.h>
#include <font.h>

#include <RTClib.h>

#include <transition.h>

#define NUM_DISPLAYS 2
#define HT1632C_BRIGHTNESS 15
#define HT1632C_DATA 9
#define HT1632C_WRCLK 8

static uint8_t HT1632C_CS[NUM_DISPLAYS] = { 5, 6 };

HT1632C ht1632c = HT1632C(HT1632C_WRCLK, HT1632C_DATA, HT1632C_CS, NUM_DISPLAYS);
Font font = Font(ht1632c);

struct Clock {
  byte hour, minute;
};

RTC_DS1307 ds1307;
Clock clock;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 253, 32);
unsigned int localPort = 8888;
IPAddress timeServer(134, 214, 100, 6);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
EthernetUDP Udp;
unsigned int last_check = 0;
#define HOUR_ADD 2

const unsigned long seventyYears = 2208988800UL;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Wire.begin();

    randomSeed(analogRead(1));

    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);

    /* LED MATRICES */
    ht1632c.setup(HT1632C_BRIGHTNESS);

    /* RTC MODULE */
    ds1307.begin(); //start RTC Clock

    Serial.println(__DATE__);
    Serial.println(__TIME__);

    if (!ds1307.isrunning()) {
        Serial.println(F("RTC is NOT running!"));
        ds1307.adjust(DateTime(__DATE__, __TIME__));
    }

    DateTime now = ds1307.now();
    clock.hour = now.hour();
    clock.minute = now.minute();

    /* SD CARD */

    pinMode(10, OUTPUT);

    if (!SD.begin(4)) {
        Serial.println(F("SD Card cannot be initialized!"));
        return; // We're fucked up
    }

    font.print_large_clock_number(11, 0, clock.hour);
    font.print_large_colon(23, 0);
    font.print_large_clock_number(27, 0, clock.minute);

    Serial.println(F("Starting ethernet..."));
    Ethernet.begin(mac, ip);
    Udp.begin(localPort);
    Serial.println(F("Done ethernet..."));

    Serial.println(F("Everything is working!"));
}

void loop() {
    DateTime now = ds1307.now();

    if (now.minute() != clock.minute) {
        clock.minute = now.minute();
        font.print_large_clock_number(27, 0, clock.minute);

        if (Transition::transition("RED.CLK", ht1632c)) {
            ht1632c.clear();
            font.print_large_clock_number(11, 0, clock.hour);
            font.print_large_clock_number(27, 0, clock.minute);
        }
    }

    if (now.hour() != clock.hour) {
        clock.hour = now.hour();
        font.print_large_clock_number(11, 0, clock.hour);
    }

    if (abs(now.minute() - last_check) > 2) {
        Serial.println(F("Go..."));
        sendNTPpacket(timeServer);
        Serial.println(F("..."));

        delay(1000);
        
        if (Udp.parsePacket()) {
            Serial.println(F("Starting..."));
            Udp.read(packetBuffer, NTP_PACKET_SIZE);

            unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
            unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
            unsigned long secsSince1900 = highWord << 16 | lowWord;
            unsigned long epoch = secsSince1900 - seventyYears;

            uint8_t h = (HOUR_ADD + (epoch % 86400L) / 3600) % 24;
            uint8_t m = (epoch % 3600) / 60;
            uint8_t s = epoch % 60;

            ds1307.adjust(DateTime(now.year(), now.month(), now.day(), h, m, s));
            Serial.println(ds1307.now().minute());
            last_check = m;
        }

        Serial.println("Done.");
    } 
}

// Send an NTP request to the time server at the given address 
unsigned long sendNTPpacket(IPAddress& address) {
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;
    packetBuffer[1] = 0;
    packetBuffer[2] = 6;
    packetBuffer[3] = 0xEC;
    packetBuffer[12] = 49; 
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    Udp.beginPacket(address, 123);
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket(); 
}
