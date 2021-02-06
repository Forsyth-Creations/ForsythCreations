This is a list of files that we have modified in the core Marlin files for our distribution.

This is for our reference to make sure to retain changes made to the core files when merging in the latest Marlin firmware baseline.

This does not include machine configs or PlatformIO files.

----------


- **Marlin**
	- **src**
		- **core**
			- `language.h`
		- **gcode**
			- **calibrate**
				- `M48.cpp`
			- **temp**
				- `M303.cpp`
		- **lcd**
			- **dwin**
				- **e3v2**
					- `dwin.cpp`
			- **menu**
				- `menu_advanced.cpp`
				- `menu_configuration.cpp`
				- `menu_motion.cpp`
			- **language**
				- `language_en.h`
		- **pins**
			- **lpc1769**
				- `pins_TH3D_EZBOARD.h`
			- **ramps**
				- `pins_RAMPS.h`
				- `pins_RAMPS_CREALITY.h`
			- **sanguino**
				- `pins_MELZI_CREALITY.h`
			- **stm32f1**
				- `pins_BTT_SKR_MINI_E3_common.h`
				- `pins_CREALITY_V4.h`
				- `pins_MKS_ROBIN_MINI.h`
	- `Configuration_adv.h`
	- `Version.h`