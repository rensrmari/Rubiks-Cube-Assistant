# Rubiks-Cube-Assistant
A C++ program that allows you to interact with a Rubik's cube and receive assistance with solving it in the terminal.

Video Demonstration: https://www.youtube.com/watch?v=b2PRqB73RsM

# Project Purpose
The purpose of this project is to provide people with an increased understanding of how to solve a Rubik's cube, a popular 3D combination puzzle. The program is designed to elaborate the process of solving the cube according to the beginner's method. You can choose to experiment with your cube in a sandbox-like environment, or you can follow along the steps provided by the assistant and see what each step entails.

# Compiling and Running
To use this program, you will need to:
1. Have Git installed: https://git-scm.com/
2. Open a terminal
3. Clone the repository
   ```
   git clone https://github.com/rensrmari/Rubiks-Cube-Assistant.git
   ```
4. Navigate to its directory
   ```
   cd Rubiks-Cube-Assistant
   ```
5. Compile the source files
   ```
   g++ main.cpp assistant.cpp cube.cpp file_handler.cpp assistant_tests.cpp cube_tests.cpp file_handler_tests.cpp tester.cpp -o [your_exe_name]
   ```
6. Run the executable
   ```
   [your_exe_name]
   ```

# Additional Features and Considerations
- The program will need to run in full screen.
- When supplying a text file of your own to store and load your cubes, they must be in the same directory as the source files.
   - More information regarding files can be found in the guide from the title screen.
- After creating a new cube or loading one in, you may begin interacting with the cube. From there, you can:
   - Access the assistant
   - Perform moves like turning and rotation and undo them
   - Save the cube to your file and exit
- To test the classes, you will need to supply an argument after the executable, indicating the class you would like to test.
   - "assistant": Test the Assistant class
   - "cube": Test the Cube class
   - "file_handler": Test the File Handler class
   - Example:
      ```
      [your_exe_name] assistant     # Testing the Assistant
      ```
- Resources used:
   - [Beginner's method](https://solvethecube.com/)
   - [Translating algorithms](https://cube.rider.biz/algtrans.html)