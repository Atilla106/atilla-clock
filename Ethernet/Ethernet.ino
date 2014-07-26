/*

 Udp NTP Client
 
 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket 
 For more on NTP time servers and the messages needed to communicate with them, 
 see http://en.wikipedia.org/wiki/Network_Time_Protocol
 
 Warning: NTP Servers are subject to temporary failure or IP address change.
 Plese check 

    http://tf.nist.gov/tf-cgi/servers.cgi

 if the time server used in the example didn't work.

 created 4 Sep 2010 
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe
 
 This code is in the public domain.

 */

#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 253, 32);

unsigned int localPort = 8888;      // local port to listen for UDP packets

// 132 163 4 101
IPAddress timeServer(134, 214, 100, 6); // time-a.timefreq.bldrdoc.gov NTP server
// IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov NTP server
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() 
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial);
  Serial.println("45");

  // start Ethernet and UDP
  //if (Ethernet.begin(mac) == 0) {
  //  Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
  //}
  Ethernet.begin(mac, ip);
  Serial.println("SWAG");
  Udp.begin(localPort);
  Serial.println("YOLO");
}

void loop()
{
  Serial.println("Go...");
  sendNTPpacket(timeServer);
  Serial.println("...");

  if (Udp.parsePacket()) {
    Serial.println("Starting...");
    Udp.read(packetBuffer, NTP_PACKET_SIZE);

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;

    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':'); 
    Serial.println(epoch % 60); // print the second
  }

  Serial.println("Done...");

  delay(10000); 
}

// send an NTP request to the time server at the given address 
unsigned long sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:         
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}