![2024-09-0320-26-21-Trim-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/32ba1825-69b7-4d03-8214-19ac0dd2e5d0)
# SFG - Simulador de Fuerzas Gravitatorias
SFG is a gravitational force simulator written in C. The simulation is composed of series of bodies floating in a 2-dimensional space, in which, they attract one another. The simulator uses Newton's law of universal gravitation for their calculations and includes a functional, although underdeveloped, collision detector.

The project comes with Graphical User Interface (GUI) developed with GTK 4 where the user can:
- **Start and stop** the simulation
- **Add new bodies** individually or in bulk
- **Adjust simulation settings**, such as the time unit per tick, the amount of space displayed on screen, or the screen's position.

This app was created as the final project for a university class. It's objective was to develop a program that optimized a certain task without the use of parallel computing. Consequently, the project has several optimizations that allow the program to run with hundreds of bodies while maintaining a reliable number of ticks per second.

**Note:** Due to the origins of the project, the GUI is currently in Spanish.
## Building the project
Once the repository is downloaded, use Meson to create the building directory:
```sh
meson "Building directory"
```

Next, enter the new directory and compile the project with Ninja.
```sh
ninja
```

**Note:** You will need to install the GTK 4 libraries for the project to compile successfully.
