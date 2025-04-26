# ESP8266 Alexa Alarm Integration

This project enables integration between a traditional house alarm system and Amazon Alexa devices, allowing remote management and status monitoring of your alarm system through voice commands.

## Features

- Control your alarm system through Alexa voice commands
- Monitor alarm armed/disarmed status
- Monitor alarm trigger status
- Automatic status updates to Alexa
- Easy integration with existing alarm systems

## How it Works

The project uses an ESP8266 microcontroller and the FauxmoESP library to emulate Philips Hue devices that Alexa can discover and control. The system creates two virtual devices:
- "Allarme" - Controls and shows the armed/disarmed status
- "Stato Allarme" - Shows the alarm trigger status

## Hardware Requirements

- ESP8266 board (NodeMCU or similar)
- Relay modules for input/output isolation
- Basic electronic components
- Access to your alarm system's status pins

## Circuit Integration

**Note**: Circuit diagram will be added in future updates.

The integration requires some basic soldering work. You'll need to:
1. Install relays to isolate the ESP8266 from the alarm system
2. Connect the armed status to PIN 4
3. Connect the alarm status to PIN 5
4. Connect the control relay to PIN 12

⚠️ **Important**: Always use relay isolation when connecting to an existing alarm system to prevent damage to either device.

## Software Setup

1. Configure your WiFi credentials in `WiFiConfig.h`
2. Flash the code to your ESP8266
3. Ask Alexa to discover new devices
4. Your alarm will appear as two light devices

## Dependencies

- ESP8266 Arduino core
- [FauxmoESP library](https://github.com/vintlabs/fauxmoESP)

## Usage

Once set up, you can:
- Say "Alexa, turn on Allarme" to arm the system
- Say "Alexa, turn off Allarme" to disarm the system
- Check the status by asking "Alexa, is Allarme on?"
- Monitor the alarm trigger status through the "Stato Allarme" device

## Safety Notice

This is a DIY project that interfaces with your security system. Make sure you:
- Understand what you're doing
- Test thoroughly before relying on it
- Maintain your existing alarm system's original functions
- Use proper isolation (relays) between circuits
