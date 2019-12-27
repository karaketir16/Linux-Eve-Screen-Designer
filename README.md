# Eve Screen Designer Linux Hal Implementation

## Example project

### Devices

- NHD_3_5C_FT813
- Colibri iMX6 
- Viola carrier board V1.2

### Preparing

- Connect pins
  
  ![pins.png](docs/pins.png)

- If needed, configure defines such as pd, cs pins in `Colibri_Hal/colibri/pinCtl.h` and spi clock speed, spi path in `Colibri_Hal/colibri/colibri_spi.h`


### Usage

- As an example `EVE Screen Designer/Examples/Basic/RadioButtonGroups` used in this project.
- To use any other project, delete `Colibri-Eve-Screen-Designer/Generated` and replace with `PathtoProject/Generated`.
- Run script.sh with `./script.sh` command
- If working on linux machine, fix lower/upper case issues.
