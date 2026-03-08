Demon Slayer: Survive the Demons
Demon Slayer: Survive the Demons is a 3D survival action game built with C++ and the Raylib framework. Inspired by the Kimetsu no Yaiba universe, the game challenges players to survive a night on Fujikasane Mountain against an escalating swarm of demons.

Gameplay Mechanics
Vitality and Breath System
Vitality (Health): Your life force. If it reaches zero, the selection ends. Vitality can be restored by standing within the aura of Wisteria Trees.

Breath (Stamina): Required for sprinting and performing Nichirin sword strikes. Managing your Breath is critical to avoid being caught stationary by a swarm.

Emergency Alerts: The Vitality bar flashes red when health drops below 20% to alert the player to seek a sanctuary.

Wisteria Sanctuaries
Strategically placed throughout the dense forest are purple Wisteria Trees. These act as safe zones that:

Provide a restorative aura for the player's Vitality.

Physically repel demons, preventing them from entering the sanctuary.

Dynamic Swarm AI
Proximity Spawning: Demons spawn in a procedural ring around the player's current position, ensuring constant engagement.

Scaling Difficulty: As the player's "Purified" score increases, the demon spawn rate and movement speed scale upward.

Aggressive Lunge: Demons switch from a chase state to a high-velocity lunge when within striking distance.

Technical Features
Custom Collision Engine: Implemented a lightweight XZ-plane circle-to-circle collision resolver for environmental obstacles (mountains) and the map perimeter.

Optimized Rendering: Uses Raylib's 3D RLGL and math headers for efficient drawing of high-density forest environments (350+ trees).

State-Based AI: Enemy logic is handled via a state-machine that manages pathfinding toward the player and sanctuary avoidance.

Controls
Installation and Setup
Prerequisites
A C++ compiler (GCC, Clang, or MSVC)

Raylib 5.0+ installed on your local machine

Building the Project
Clone the repository:
git clone https://github.com/Mukund14K/Demon-Slayer-Game.git

Compile the source code (example using G++):
g++ main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o demon.exe

Run the generated executable.
