# Tamariw

This branch contains the embedded source code to interface with the [ground station for Tamariw](https://github.com/TAMARIW/gs).

## schematics of indices

```
     _____________       ______ ______
    |             |     |      -      |
    |  1       0  |     |  3       2  |
    | (O)     (O) |     | (O)     (O) |
    |             |     |             |
    |      1      |     |      3      |
    |      o      |     |      o      |
    |             |     |             |
    |             |     |             |
    | 0 o     o 2 |     | 2 o     o 0 |
    |             |     |             |
    |             |     |             |
    |      o      |     |      o      |
    |      3      |     |      1      |
    |             |     |             |
    |             |     |             |
    |  2       3  |     |  0       1  |
    | (O)     (O) |     | (O)     (O) |
    |______-______|     |_____________|

(O) - electromagnet
 o  - time of flight sensor
```

## Commands

1. To cache the host key of the Raspberry Pi to transfer `main.hex`.
```
pscp -pw tamariw build/main.hex tamariw@tamariw.local:/home/tamariw/tpi/src
plink.exe tamarix@tamarix.local
```

# Todos

1. Implement the KF tuner in STM32 side.
2. Debug the issue with ToF sensor measurements.
3. How to get away with that one prompt that appears for the first time when using `pscp` on GS.
4. Implement the status flag for the Kalman Filter.
5. Ground station docuking params: Kp, Ki, Kd, distance set point, latching current, latching and unlatching buttons.
