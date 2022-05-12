<h2>This is a in-class project for COM S 327 - Advance Programming Techniques, Spring 2022. </h2>
<h4>
This project that I had worked on is a simplified version of Pokemon that is playable through the console. The programme should be ran on a linux machine as it uses nCurses as its primary interface API.
</h4>

---
  
<p>
The final version that I've submitted is v1.09 and implemented a few additional features in addition to what was required.

Below are the implemented part of my game:
  - All Pokemon generations are available.
  - All movesets from all generations are available
  - Pokemon level up, Pokemon stat changes as you level up.
  - Simplified PokeCenter and PokeMart.
  - Pokemon Storage System (accessed through PokeCenter).
  - Wild and Trainer Battle.
  - Damage calculation, which includes: critical strike, missrate, and matching type bonus.
  - Terrain generation (Procedurally generate a map each time the player enters a new map, this includes building)
  - Spawning buildings (The moment the player spawns into the game map, PokeMart and PokeCenter has a 100% spawn rate. This chance decreases as you move further from the center).
  
  
Below are things that aren't implemented due to the time constrain and work load of the semester.
Things that are not implemented from the original game:
  - Specific spawn locations for Pokemon (Every type of Pokemon has a random chance of spawning! And yes, that includes legendaries).
  - Leveling up moveset (Moves currently cant be learnt during level up).
  - Pokemon do not evolve.
  - Pokemon Move type effectiveness is not calculated.
  - Majority of items are not in the game, the only 3 I have implemented is: Pokeball, Revives, Potion.
  - No Story or quests lines.
</p>

---

<h3> Charaters/Trainers </h3>

  | Character/Letter | Name | Type | Description |
  | --- | --- | --- | --- |
  | @ | Ash | Player | User has full control over the Ash, able to move in 8 directions. Look at controls for more details. Ash cannot travel through snowy mounts, trees, water and bolders. |
  | h | Hiker | NPC | Hikers always path find their way to the player. They move faster on grass and move the same speed as Ash on pathways. Atleast one hiker exist on each map. Hikers can traverse in all type of terrain except water/lake. |
  | r | Rival | NPC | They move similar to hiker, but they have the same movement speed as Ash and also always have atleast one in each map. |
  | p | Pacer | NPC | Pacers moves back and forth bounded by the terrain type they spawned on. |
  | w | Wanderer | NPC | Wanderer moves in a square/rectangular shape when possible in each direction. (N, E, S, W) |
  | s | Stationaries | NPC | Stationaries do not move from their spot, but can initiate battle if Ash runs into them. |
  | n | Random Walker | NPC | Random Walker has similar movement as wanderer, the unique trait is that they are not bounded by the terrain type that they spawned on. |


<h3> Terrain Types/Buildings </h3>

  | Character/Letter | Name | Description |
  | --- | --- | --- |
  | ' | Grass 1 | Have a chance to encounter wild Pokemon. Ash moves slower in this tile. |
  | ; | Grass 2 | Have a chance to encounter wild Pokemon. Ash moves slower in this tile. |
  | - | Grass 3 | Have a chance to encounter wild Pokemon. Ash moves slower in this tile. |
  | " | Grass 4 | Have a chance to encounter wild Pokemon. Ash moves slower in this tile. |
  | ^ | Tree | Only Hiker can traverse through this terrain. All other trainers cannot travel through this tile. |
  | * | Snow Mountains | Only Hiker can traverse through this terrain. All other trainers cannot travel through this tile. |
  | ~ | Water/Lake | For now, no one can travel through this tile. |
  | % | Wall/Boulders | No one can travel through this tile. |
  | = | Exits | Exits for Ash to travel to another map. |
  | # | Paths | Path ways for Ash to travel through without interference from wild Pokemons. |
  | M | PokeMart | Building for the player to purchase and sell items. |
  | C | PokeCenter | Building for the player to heal and transfer Pokemons from storage system. |

---

<h3>
How to run programme on a linux machine:
</h3>

  - run the MakeFile using the "make" command.
  - then execute the executable using "./map"
  - (optional) You can pass in unique switch cases as command line argument: 
      - "--seed" : allows the player declare a specified seed.
      - "--numtrainers" : allows the player to declare a specified amount of trainers that could appear in a map. Set to 8 by default.

---

<h3>
Gameplay Screenshots:
</h3>

<h4> Map </h4>

![Gameplay Map 1](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console1.png?raw=true "Gameplay Map 1")
![Gameplay Map 2](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console12.png?raw=true "Gameplay Map 2")

<h4> Battles </h4>

![Wild Battle](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console2.png?raw=true "Wild Battle")
![Trainer Battle 1](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console7.png?raw=true "Trainer Battle 1")
![Trainer Battle 2](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console8.png?raw=true "Trainer Battle 2")

<h4> Swapping Pokemon </h4>

![Swapping Pokemon](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console3.png?raw=true "Swapping Pokemon")

<h4> Bag/Inventory </h4>

![Bag/Inventory](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console4.png?raw=true "Bag")

<h4> PokeCenter & Storage </h4>

![PokeCenter](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console5.png?raw=true "PokeCenter")
![Pokemon Storage](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console6.png?raw=true "Pokemon Storage")

<h4> PokeMart, Buying, Selling </h4>

![PokemonMart 1](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console9.png?raw=true "PokeMart 1")
![PokemonMart 2](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console10.png?raw=true "PokeMart 2")
![PokemonMart 3](https://github.com/Brandon-YH/COMS327/blob/main/Gameplay%20Screenshot/Console11.png?raw=true "PokeMart 3")

