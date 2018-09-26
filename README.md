# DreamStream

***DISCLAIMER: This stuff is WIP! AKA: Extremely early and hacky in some parts and should as of now not be used except as code reference.***

The goal of this library is to provide a cross-platform, open source implementation of the DreamScreen protocol, to allow many different devices to use the RGB data coming from the screen, and to fill the connectivity gap that the device itself is lacking!

### Long-term goals:
- Support for Razer Chroma, Philips Hue, more.
- Control emulated and external devices using MQTT

### Short term goals:
- Arduino support (Wi-Fi enabled) (plug and play firmware binaries)

### Features as of now:
- manage and control remote devices (although very untested atm)
- works with the official app
- subscribes to and receives sector data
- pretty complete implementation of DreamScreen / SideKick state payloads

### Todo:
- extremely early, so expect hacks here and there
- fix up windows version (currently only runs on linux)
- actually do something with the sector data
- store device info in some config file
- make devices connect to host when streaming sectors
- clean up includes
- remove (big) hack to stop messages broadcasted to ourselves (device manager), switch to multicast?
- fix up warnings
- better application lifecycle
- start porting to arduino!

### Build instructions:
- none, do not run this yet lol

### Credits
- DreamScreen LLC
- Robert Koszewski for his initial Java implementation: https://github.com/rkkoszewski/DSComEmu
