# Water Shooting Robot

An Arduino-based mechatronic system that aims, elevates, and fires controlled water bursts at predefined targets. This repository now has a cleaned structure separating firmware, hardware (CAD), and documentation to make iteration and collaboration easier.

## Key Features
- Automated targeting sequence (distance + angle arrays)
- Stepper-driven vertical lift platform
- Servo-based nozzle aiming
- Solenoid valve (relay) water burst control
- Basic ballistic height estimation with dynamic water level model
- Modular test sketches for hardware bring-up

## Repository Structure
```
firmware/
  prototype/           # Main consolidated prototype firmware (water_robot_v1.ino)
  tests/               # Focused hardware test sketches (stepper, etc.)
hardware/
  README.md            # Notes + structure guidance for CAD assets
docs/
  wiring.md            # Pin map & wiring diagrams (placeholder)
.gitignore             # Arduino + CAD temp artifacts ignored
README.md              # You are here
```

### Legacy Files (Pending Migration)
The original `src/` folder and `CAD-models/` are left in place for reference. After validating the new structure you can:
1. Move DWG / CAD assets into `hardware/cad/` (create it).
2. Remove noisy or corrupted sketches (e.g. malformed line at top of `Code_v2.0.ino`).
3. Delete `src/` once nothing unique remains.

## Hardware Overview
| Component            | Notes |
|----------------------|-------|
| Microcontroller      | Arduino (Uno / Nano assumed) |
| Stepper + Driver     | NEMA + driver (e.g. A4988 / DRV8825) |
| Servo                | Standard hobby servo for nozzle rotation |
| Solenoid Valve       | Driven via relay or MOSFET board (flyback protection required) |
| Lift Mechanism       | Lead screw / linear stage (calibration required) |
| Power                | Separate motor vs. logic rails recommended |

Fill in exact part numbers for reproducibility.

## Control / Math Notes
The firmware estimates required nozzle height using:
```
requiredHeight = Y_BOTTLE + 0.25 * x^2 * (1 / waterHeight)
```
Water depletion after each shot uses a Torricelli-inspired relation:
```
z_new = ( sqrt(z_old) - sqrt(g/2) * (Aj/Ab) * t_open )^2
```
These are simplifications—empirical tuning will be needed. Consider moving constants to a dedicated header later.

## Firmware
Primary sketch: `firmware/prototype/water_robot_v1.ino`

Test sketches live in `firmware/tests/` (e.g. `stepper_lift_test.ino`). Add more for each actuator (servo test, valve timing, etc.).

### Pin Assignments (Prototype)
| Function      | Pin |
|---------------|-----|
| Stepper DIR   | 3 |
| Stepper STEP  | 4 |
| Servo (nozzle)| 10 |
| Relay (valve) | 11 |

Document changes in `docs/wiring.md` if you rewire.

## Getting Started
1. Clone the repo
2. Open `firmware/prototype/water_robot_v1.ino` in Arduino IDE (or VS Code + Arduino extension)
3. Select board + port
4. Ensure `Servo` library (bundled) is available
5. Upload & open Serial Monitor (9600 baud)

### Calibration Steps
1. Measure vertical travel per full step (or steps per mm)
2. Update `displacementToSteps()` in `water_robot_v1.ino`
3. Log serial output and compare predicted vs. actual height
4. Tune `openTime` (valve duration) and target arrays

### Safety & Testing Tips
- Start with minimal water
- Keep electronics splash-protected
- Provide an emergency power or valve cutoff
- Add a drip tray during iteration

## Suggested Roadmap
- Replace hard-coded targets with serial command parser (e.g. JSON or CSV input)
- Add calibration routine (interactive via serial)
- Introduce finite state machine (IDLE, AIM, FIRE, REFILL, ERROR)
- Add feedback sensors (float switch, pressure, encoder)
- Abstract constants into a config header file
- Add photos / short GIF demo to README

## Contributing
1. Fork & branch (`feature/your-feature`)
2. Keep commits focused (hardware vs. firmware vs. docs)
3. Add/Update documentation for new pins or formulas
4. Run actuator tests where relevant before opening PR

## License
Add a license (MIT / Apache-2.0 / GPL) to clarify usage. Currently unspecified.

---
Maintained by Kronbii — ideas & PRs welcome.
