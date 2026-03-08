**Spirit Hunter: Breath of the Night**
Spirit Hunter: Breath of the Night is a 3D survival action game built with C++ and the Raylib framework. Set in a dense, mystical forest, the game challenges players to survive a night against an escalating swarm of malevolent spirits using unique breathing techniques and environmental strategy.

**Gameplay Mechanics**
1.Vitality and Breath System
-Vitality (Health): Your life force. If it reaches zero, the hunt ends. Vitality can be restored by standing within the restorative aura of Sacred Trees.

-Breath (Stamina): Required for high-speed sprinting and sword strikes. Managing your Breath is critical; running out leaves you vulnerable to a swarm.

-Emergency Alerts: The Vitality bar flashes red when health drops below 20% to warn the player to retreat to a sanctuary.

2.Sacred Sanctuaries
-Strategically placed throughout the forest are Purple Wisteria Trees. These act as safe zones that:

-Provide a restorative aura for the player's Vitality.

-Physically repel spirits, preventing them from entering the sanctuary.

3.Dynamic Swarm AI
-Proximity Spawning: Spirits spawn in a procedural ring around the player, ensuring constant engagement without overwhelming the hardware.

-Scaling Difficulty: As your Purified score increases, enemy spawn rates and movement speeds scale upward.

-Aggressive Lunge: Enemies switch from a chase state to a high-velocity lunge when within striking distance.

**Technical Features**
Custom Collision Engine: Implemented a lightweight XZ-plane circle-to-circle collision resolver for environmental obstacles and map perimeters.

Optimized Rendering: Uses 3D RLGL and math headers for efficient drawing of high-density forest environments (350+ trees).

State-Based AI: Enemy logic is handled via a state-machine that manages pathfinding and sanctuary avoidance.

Controls
WASD: Movement

SHIFT: Sprint (Consumes Breath)

MOUSE LEFT: Attack

MOUSE MOVE: Rotate Camera

R: Restart (on Game Over)

Installation and Setup
Prerequisites
A C++ compiler (GCC, Clang, or MSVC)

Raylib 5.0+ installed on your local machine

Building the Project
Clone the repository:
git clone https://github.com/Mukund14K/Spirit-Hunter-Game.git

Compile the source code (Example using G++):
g++ main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o demon.exe

Run the generated executable.
