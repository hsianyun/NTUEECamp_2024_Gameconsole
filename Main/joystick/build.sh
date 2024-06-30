   
cd build || exit

# Run CMake configuration with Ninja
cmake .. -G Ninja

# Build the project
ninja

# Flash the project
ninja flash

# Start the monitor
ninja monitor

