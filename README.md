# ESP32-CaptivePortal-RickRoll

This Arduino sketch transforms a compatible ESP32 microcontroller into a captive portal that tricks connected devices into a RickRoll.

## Features:

 * Creates a WiFi access point (AP)
 * Implements a DNS server to redirect all requests to the local IP address
 * Responds to captive portal detection URLs from Apple, Google, and Microsoft
 * Responds to all other requests with self-contained RickRoll html
