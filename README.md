# homeassistant-waybar
A module for Waybar that can show sensor values from Homeassistant

# Dependencies

- Cmake
- Curl
- Homeassistant with enabled Rest Api <em>([link1](https://developers.home-assistant.io/docs/api/rest/) and [link2](https://www.home-assistant.io/integrations/api/) to the docs)</em>

# Build
You need Cmake, make and curl
```Bash
cmake -S src -B build
cd build
make
```
Now you can start using the binary

# Writing the Config File
take a look at the examples in the [example folder](examples)

only required value is the Path to your sensor
