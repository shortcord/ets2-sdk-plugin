# ETS2/ATS Telemetry Plugin

This repository is a fork of the original [ets2-sdk-plugin](https://github.com/nlhans/ets2-sdk-plugin), adapted for [ETS2 Telemetry Web Server](https://github.com/Funbit/ets2-telemetry-server).

It exposes SCS telemetry data through a shared-memory map so external clients can read live truck, trailer, job, and navigation state.

## Overview

- **Game support:** Euro Truck Simulator 2 / American Truck Simulator
- **Plugin name:** `ets2-telemetry-server`
- **License:** MIT

## Build

### Linux

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The Linux build produces `ets2-telemetry-server.so`. Copy it to the game's Linux plugin directory, for example `bin/linux_x64/plugins/`.

### Windows

- Visual Studio solution: `ets2-telemetry/ets2-telemetry.sln`
- Visual Studio project: `ets2-telemetry/ets2-telemetry.vcxproj`
- Output: `ets2-telemetry-server.dll`

## Shared memory

The telemetry map is stored in a fixed-size shared-memory block.

- **Windows name:** `Local\Ets2TelemetryServer`
- **Linux name:** `/Ets2TelemetryServer`
- **Size:** `ETS2_PLUGIN_MMF_SIZE` (`16 * 1024` bytes)

Clients should read the `ets2TelemetryMap_t` layout from `ets2-telemetry/inc/ets2-telemetry-common.hpp`.

Notes:
- `tel_revId` identifies the plugin revision and game version.
- `tel_rev1` through `tel_rev4` are packed into one binary-compatible structure.
- Some string data is stored in fixed 64-byte fields.
- The `modelType` and `trailerType` fields use offsets into the shared-memory block for string storage.

Example Linux client access:

```c
int fd = shm_open("/Ets2TelemetryServer", O_RDONLY, 0);
```

## Runtime flow

At startup the plugin:
1. Creates or opens the shared-memory block
2. Clears it and writes default values
3. Registers telemetry event callbacks
4. Registers telemetry channels into shared-memory fields
5. Registers configuration handlers for truck/job metadata

During frame updates it:
- advances the shared timestamp
- mirrors pause state
- updates cruise-control state
- clears or restores job-related fields when jobs finish or trailers are attached

## Telemetry mapping

### Core vehicle state (`tel_rev1`)
- trailer attachment
- vehicle speed, acceleration, position, rotation
- gear, gear count, engine RPM and max RPM
- fuel, fuel capacity, fuel consumption
- user input and effective input values
- truck/trailer weights and model/trailer type references

### Job / trailer metadata (`tel_rev2`)
- game time (`time_abs`)
- reverse gear count
- trailer mass, ID, display name
- income, delivery deadline, source/destination city and company

### Controls and warnings (`tel_rev3`)
- retarder, shifter, wipers, parking brake, motor brake
- electric/engine enabled state
- blinkers and lights
- warning flags (air pressure, oil pressure, water temp, battery, AdBlue)
- wear values, odometer, cruise-control speed
- truck make, make ID, and model strings

### Advanced state (`tel_rev4`)
- warning thresholds and retarder step count
- cabin/head/hook positions
- shifter type
- local time scale and next rest stop
- trailer placement and navigation fields
- displayed gear

## Configuration handling

Configuration events populate data that is not available as a per-frame channel:

- truck brand/model IDs
- cargo/trailer identifiers
- job income and deadline
- cargo mass and route metadata
- warning thresholds
- cabin/head/hook positions
- shifter type

Job data is buffered so it can be restored when the trailer is reattached after delivery.

## Known limitations

- Some fields are marked deprecated or TODO in the source.
- Wheel/slot configuration support is present in comments only.
- String handling relies on fixed buffer sizes and shared-memory offsets.
- The shared-memory schema is a binary contract; changing it breaks clients.

# Version history

### Revision 10 (19th June 2016)

- implemented job information buffer to avoid unexpected reset when trailer is detached on the way

### Revision 9 (28th March 2016)

- refactored job information reset, should work a bit more stable

### Revision 8 (13th March 2016)

- fixed job information reset when job is done

### Revision 5 (13th June 2015)

- removed duplicated engine\_enabled property which was a duplicate for electric\_enabled property causing it not to work properly

### Revision 4 (23th May 2015)

- updated for SCS SDK 1.5
- added support for new telemetry data
- changed MMF name to "Local\\Ets2TelemetryServer"

# License

The MIT License (MIT)
