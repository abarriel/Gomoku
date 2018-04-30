# [GOMOKU](https://cdn.intra.42.fr/pdf/pdf/105/gomoku.pdf)

**Make an AI capable of beating human players at Gomoku**

![
](https://user-images.githubusercontent.com/25068709/39411763-ed04983e-4c10-11e8-82e5-3aa521d68ca3.png)

## Technical

- **Min Max Algorithm** (NegaMax implementation) the subject required the algorithm to finish in less than 0.5 seconds, thus **depth** is set to **4**. [pdf](https://cdn.intra.42.fr/pdf/pdf/105/gomoku.pdf)
- **Memory Alpha Beta Pruning** [pdf](https://pdfs.semanticscholar.org/c876/c5fed5b6a3a91b5f55e1f776d629cc8ed9bc.pdf)
- **SDL2**
-  In Gomoku, there is a [game strategy](http://gomokuworld.com/gomoku/1)  call VCT (Victory by Continuous Threats), at some point, we are sure to win if we keep attacking, the opponent wouldn't have any option, either to block the four, double three or lose. We are looking for this particular win position. We use a different heuristic and our **Min Max Algorithm** with **depth** set to **12**

## Strategies
* Due to the time restrictions, we use a lot of imagination to optimize our algo the best we can. We check the board using **bitwise** for our heuristic for example.
	```
	11: opponent's stone
	01: my stone
	00: empty place
	```
	a slice of the board `1100 0011 1101 0101` means `O . . O O X X X`
* we're **sorting moves** every time, the prunning is therefore more efficient
* We use a **12 depth Min Max Algorithm** for the best moves. We used **multithread** to optimize the time (up to **4** in **parallelism**)

## Program
 Modes: **pro - long-pro - swap - swap2 - standard**
Options: **capture mode, ending capture, no double three, swap opportunity** 
You can replay the full party, and get a debug mode, with a full detail heuristic.
We haven't spend too much time doing a nice menu, everything work with CLI. 

## Installation

Due to our school, and our restricted privileges. We added the SDL2 and SDL2_image library compiled in the `{PROJECT}/sdl/`. To run it on a different system, you should update the Makefile a bit, remove `-L sdl/lib -I sdl/include`.



🔥 **Our AI is very aggressive and he is unbeatable !!**
