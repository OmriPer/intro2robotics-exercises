Submission date: 10.11.26 (23:59)
# Prerequisites
make sure argos3 is installed:
```sh
argos3 -q all
```
the expected output is a printout of all available plugins (there are a lot).
- if argos isn't installed - refer to https://github.com/OmriPer/pipuck_sim/blob/main/docs/installation.md
# Exercise Instructions
Write a controller that:
- drives forwards until there is an obstacle (wall) 10 cm away.
- When it senses the obstacle, it should turn in place until the way is clear, and then keep driving forwards.
- The LEDs should be lit in green while driving, and red while turning.
# Work Space Structure
The `ex1_ws` Repository - https://github.com/OmriPer/ex1_ws
contains 3 directories:
- **`controllers/`**: Contains the source code for robot controllers.
- **`experiments/`**: Contains the `.argos` XML configuration files for simulations.
- **`loop_functions/`**: Contains custom loop functions to manage experiment logic.

You only need to write your code under the `controllers/` directory. \
**Don't modify** any other directory - so your setup is identical to the test setup.
# Rules
1. Make sure your code is tidy and well-documented.
2. Make sure your ID is written at the top of any file you modify.
3. Name any source you used in your README file.
# Files to submit
1. You should submit your project in the same structure as `ex1_ws` (*zipped*).
2. Add a README file with your ID, and a list of any external source you used.
3. **Do not submit any executable files, or any files that can be regenerated** (for example the `build/` directory).
# Self Test
For self-examination you can:
1. compile your workspace
`./compile_ws.sh`
2. run the experiment
`argos3 -c experiments/config1.argos`
3. check the robot behaves as expected
- if this process fails, it will probably fail in the test environment as well.