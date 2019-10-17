# Eve Screen Designer Colibri Hal Implementation

### Preparing
- Include files under `Colibri_Hal` folder to project.

- Add defines to .pro file
  ```
  DEFINES += ColibriiMX6_PLATFORM
  DEFINES += NHD_3_5C_FT813
  DEFINES += DISPLAY_RESOLUTION_QVGA
  DEFINES += EVE_GRAPHICS_AVAILABLE
  DEFINES += EVE_DISPLAY_AVAILABLE
  DEFINES += FT813_ENABLE
  ```
- Connect pins
  
  ![pins.png](docs/pins.png)

- If needed, configure defines such as pd, cs pins or spi clock speed in `Colibri_Hal/colibri_spi/colibri_spi.h`


### Using

- As an example `EVE Screen Designer/Examples/Basic/RadioButtonGroups` used in this project.
- Copy `RadioButtonGroups/Generated` to `desginerOut/`
- Rename `Generated/ME812A WH50R` to `Generated/ME812A_WH50R`
- Rename `Generated/ME812A_WH50R/RadioButtonGroups` to `Generated/ME812A_WH50R/App`
- If working on linux machine, fix lower/upper case issues.