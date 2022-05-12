<h2>This is a in-class project for COM S 327 - Advance Programming Techniques, Spring 2022. </h2>
<h4>
This project that I had worked on is a simplified version of Pokemon that is playable through the console. The programme should be ran on a linux machine as it uses nCurses as its primary interface API.
</h4>

  
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

<h3>
How to run programme on a linux machine:
</h3>
<p>
  - run the MakeFile using the "make" command.
  - then execute the executable using "./map"
  - (optional) You can pass in unique switch cases as command line argument: 
      - "--seed" : allows the player declare a specified seed.
      - "--numtrainers" : allows the player to declare a specified amount of trainers that could appear in a map. Set to 8 by default.
</p>
