
# nPM1100 Evaluation Board – Circuit and PCB Explanation

## Overview

This project is a compact evaluation PCB designed for the **nPM1100 Power Management IC** by Nordic Semiconductor. The board provides the essential circuitry to evaluate the core features of the PMIC in a minimal footprint while exposing all IO pins for external interfacing.

- **PCB dimensions**: 22 mm × 16 mm  
- **PCB layers**: 2  
- **All components**: Surface-mounted on the top layer  
- **Header pitch**: Standard 2.54 mm (0.1")


## Functional Overview

### 1. Power Input and Charging

- Power is supplied to the board through a 2-pin header (`J1`) connected to the `VBUS` input of the nPM1100.
- Battery connects via a 3-pin header (`J2`), which routes to the `VBAT` pin of the IC.
- Charging current is configured using a **1.5 kΩ resistor** (`R_ICHG1`) connected to the `ICHG` pin.

### 2. BUCK Output

- The buck converter output `VOUTB` is enabled and its voltage is set using `VOUTBSET1` and `VOUTBSET2` . These are wired to fixed levels to select a predefined output voltage to `3.0v`.
- The regulated output is available on the `VOUT` net and accessible through the pin headers.

### 3. Status Indication

- Two LEDs are included to indicate charging and error status:
  - `LD1` (`CHG` indicator): Connected to the `CHG` pin via a 10 kΩ pull-up resistor (`R6`) to indicate battery charging.
  - `LD2` (`ERR` indicator): Connected to the `ERR` pin via a 10 kΩ pull-up resistor (`R7`) to indicate fault/error states.

### 4. Ship Mode Control

- A push-button switch (`SW1`) is connected between `SHPHLD` and ground.
- A 100 kΩ pull-up resistor (`R5`) keeps `SHPHLD` high when the button is unpressed.
- This allows the user to manually exit ship mode, as per the nPM1100 datasheet.

### 5. Voltage Monitoring Circuit

- A battery voltage monitoring circuit is implemented using a **dual N-Channel MOSFET** (`Q1`: DMC2400UV).
- The circuit is controlled by an external enable signal from the header pin `VMon_E`.
- When `VMon_E` is **high**, the MOSFETs turn on, enabling the path from `VBAT` through a resistive voltage divider to the `V.SIG` header.
  - **Voltage divider resistors**:
    - `R1`: 1.5 MΩ  
    - `R2`: 220 kΩ
- This produces a scaled version of the battery voltage suitable for external ADC measurement.
- The circuit remains off during low-power operation unless explicitly enabled by the external MCU.

### 6. IO Accessibility

- All key IO pins from the nPM1100 are broken out to standard 2.54 mm headers for easy interfacing:
  - Power and system rails: `VBUS`, `VBAT`, `VOUTB`
  - Control and status signals: `MODE`, `ISET`, `ERR`, `CHG`, `SHPACT`, `VMon_E`, `V.SIG`

## PCB Layout

- Silkscreen labels clearly identify all components and headers.
- The compact footprint (22 mm × 16 mm) is achieved through careful component placement and dense routing.
- The board outline is defined in the `Edge.Cuts` layer.


## PCB Images

### Top View
![Top View](https://i.ibb.co/RphNXJW7/frontcrop.jpg)
### Bottom View
![Bottom View](https://i.ibb.co/fdS4YsFV/backcrop.jpg)

## Summary

This evaluation board provides a minimal, efficient platform for evaluating and integrating the nPM1100 PMIC into embedded systems. It includes all supporting components for power regulation, battery charging, and system voltage monitoring, while keeping the design compact and practical for prototyping.

**Designed by: Parth Sahni**
