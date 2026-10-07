Submission date: 1.12.26 (23:59)
# Prerequisites
make sure argos3 is installed:
```sh
argos3 -q all
```
the expected output is a printout of all available plugins (there are a lot).
- if argos isn't installed - refer to https://github.com/OmriPer/pipuck_sim/blob/main/docs/installation.md
# Exercise Instructions
Write a controller that finds a target, and then drives back to where it started.

The robot starts somewhere in an arena with obstacles. The target is a red light. The robot does not know where the target is, and it does not know where the obstacles are.
## What the robot should do
1. **Search**: wander around the arena, without touching anything, until the camera sees the red light.
2. **Approach**: drive to the light, and get within 10 cm of it.
3. **Return**: drive back to the position it started from, and stop there.

On the way back the robot may only drive through grid cells it has already driven through. Those are the only cells it knows are free. This means the robot has to build its own map while it searches, and plan a path on that map.
## The world
- The arena is always 4 m x 4 m, centered at (0, 0).
- Think of it as an 8 x 8 grid of 0.5 m cells. Every obstacle fills exactly one cell.
- The start position, the target position and the obstacles are different in the test setup. **Don't hard-code** any of them.
- The robot has a positioning sensor, so it always knows where it is.
- The camera sees the red light from about 1.4 m away, unless an obstacle is in the way. For every light it sees it gives the color, the angle (relative to the robot's front) and the distance (in cm).
## LEDs
The LEDs show what the robot is doing:

| State | LED color |
|---|---|
| Searching | Yellow |
| Approaching the target | Green |
| Returning home | Blue |
| Back home | White |

The robot's LEDs must never be red.
## What counts as success
The referee (the loop functions in the workspace) checks three things:
1. The robot got within 12 cm of the target.
2. After that, it got within 5 cm of its start position.
3. It never touched a wall or an obstacle.

The robot has 30 minutes of simulated time.
## Hints
- You may reuse code from the tutorials' example repository (https://github.com/OmriPer/intro2robotics-examples). If you do, say so in your README.
- Write the controller as a finite state machine, as in Tutorial 3.
- Mark the cell the robot is in as "free" at every step. That is your map.
- To plan the way home, run A* (Tutorial 4) where a cell is allowed only if the robot has driven through it.
- The start position is usually not the center of a cell. Make it the last point of your path.
# Work Space Structure
The `ex2_ws` Repository - https://github.com/OmriPer/ex2_ws
contains 3 directories:
- **`controllers/`**: Contains the source code for robot controllers.
- **`experiments/`**: Contains the `.argos` XML configuration files for simulations.
- **`loop_functions/`**: Contains the referee, and the code that draws the robot's path.

You only need to write your code under the `controllers/` directory.
**Don't modify** any other directory - so your setup is identical to the test setup.
# Rules
1. Make sure your code is tidy and well-documented.
2. Make sure your ID is written at the top of any file you modify.
3. Name any source you used in your README file.
# Files to submit
1. You should submit your project in the same structure as `ex2_ws`.
2. Add a README file with your ID, and a list of any external source you used.
3. **Do not submit any executable files, or any files that can be regenerated** (for example the `build/` directory).
# Self Test
For self-examination you can:
1. compile your workspace
`./compile_ws.sh`
2. run the two example arenas
`argos3 -c experiments/config2a.argos`
`argos3 -c experiments/config2b.argos`
- if this process fails, it will probably fail in the test environment as well.

The experiment stops by itself when the robot is back home. The referee then prints its verdict in the ARGoS log window, ending with `PASS` or `FAIL`.

The window draws the path the robot drove, in the color its LEDs had at each point. A black ring marks the start, and a red ring marks the area around the target that counts as "reached".

A random walk is different every time. To try another one, change `random_seed` at the top of the `.argos` file. Your controller will be tested with several seeds, in arenas you have not seen.
# Grading
| Part | Weight |
|---|---|
| Reaches the target without touching anything | 40% |
| Returns home, driving only through cells it had already driven through | 35% |
| Correct LED color in every state, tidy and documented code, and a README | 15% |
| the way home is at most 1.2 times the shortest grid path, in at least 4 of 5 test runs | 10% |

The referee decides whether the robot reached the target, returned home and touched anything. The LED colors and the cells driven through on the way home are checked manually, from the path drawn in the window and from your code.

The referee prints both lengths at the end of every run in which the robot returned home.