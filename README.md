# 🎮 42 Game Dev Peer Class Curriculum

This curriculum takes you from basic terminal I/O to building a complete **2D terminal game**.  
You’ll learn graphics (via ANSI codes), input handling, interactive systems, and engine-like architecture — all inside the terminal.

---

## ⚠️ Before you start ⚠️ 

Request access to the repository and create a branch with your intra name.
```bash
git clone git@github.com:42GDC/terminal_game.git
cd terminal_game
git switch -c your_intra_name
```

---

## 📂 Terminal Game (No Graphics)

---

### **ex00: Display Map from File**
**Goal:**  
Load a map from a `.txt` file (e.g., `1` = wall, `0` = floor, `P` = player).
The maps are available in the assets subdirectory.

**Compare:**
```bash
./example00 ../assets/map1.txt
```

**Prototype:**
```c
#define MAP_WIDTH 15
#define MAP_HEIGHT 8

typedef struct s_map
{
	char	m[MAP_HEIGHT][MAP_WIDTH];
} t_map;

void print_map(t_map *map);
void read_map(char *filename, t_map *map);

int main(int ac, char *av[])
{
	if (ac == 2)
	{
		t_map map;
		read_map(av[1], &map);
		print_map(&map);
	}
	return 0;
}
```

<details>
<summary>💡 Hint</summary>

Use `open`, `read`, and `close` to parse file contents into a 2D array.  
Validate map integrity and enforce boundary constraints.  
[File I/O in C](https://www.learn-c.org/en/File_Input/Output)
</details>

---

### **ex01: Draw in a Box**
**Goal:**  
Enclose the printed map in a **box**, draw elements with spaces and shaded characters, and print the map name centered above the box.

**Compare:**
```bash
./example01 ../assets/map3.txt
```

**Prototype:**
```c
typedef struct s_map
{
	char	*name;
	char	m[MAP_HEIGHT][MAP_WIDTH];
} t_map;

void print_map(t_map *map);
```

<details>
<summary>💡 Hint</summary>

Use Unicode [box-drawing characters](https://en.wikipedia.org/wiki/Box-drawing_characters) for the frame.
</details>

---

### **ex02: Add Colour with ANSI Escape Codes**
**Goal:**  
Colour-code different map elements (walls, player, goals).

**Compare:**
```bash
./example02 ../assets/map4.txt
```

<details>
<summary>💡 Hint</summary>

Use `\033[31m` (red), `\033[32m` (green), etc., and reset with `\033[0m`.  
Draw every square as `"██"` to make it solid.  
[ANSI Colour Codes](https://en.wikipedia.org/wiki/ANSI_escape_code)
</details>

---

### **ex03: Read Terminal Input and Move Player**
**Goal:**  
Move the player with **WASD** keys.  
Quit if the player reaches the exit or presses `Q`.

**Compare:**
```bash
./example03 ../assets/map1.txt
```

**Prototype:**
```c
typedef struct s_map
{
	char	*name;
	char	m[MAP_HEIGHT][MAP_WIDTH];
	int		px;
	int		py;
	int		key;
} t_map;
```

```c
void game_loop(t_map *map);
```

<details>
<summary>💡 Hint</summary>

Capture input using:
```c
read(STDIN_FILENO, &c, 1)
```
Set terminal to raw mode with `termios` so Enter is not required.  
Add collision detection for walls.
</details>

---

### **ex04: Clear Terminal Before Redraw**
**Goal:**  
Redraw the map cleanly on each update and hide the cursor.

**Compare:**
```bash
./example04 ../assets/map1.txt
```

<details>
<summary>💡 Hint</summary>

Clear with:
```c
#define CLEAR_SCREEN   "\033[2J\033[3J\033[H"
```
Cursor:
```c
#define HIDE_CURSOR    "\033[?25l"
#define SHOW_CURSOR    "\033[?25h"
```
</details>

---

### **ex05: Implement Map Wrapping**
**Goal:**  
If the player moves off one edge, they appear on the opposite side.

**Compare:**
```bash
./example05 ../assets/map2.txt
```

<details>
<summary>💡 Hint</summary>

Use modulo arithmetic for wrapping:
```c
px = (px + MAP_WIDTH) % MAP_WIDTH;
```
</details>

---

### **ex06: Collectibles and Score**
**Goal:**  
Make the player collect `C` items, each worth **1000 points**.  
Display score at the top of the screen.

**Compare:**
```bash
./example06 ../assets/map4.txt
```

**Prototype:**
```c
typedef struct s_map
{
	char	*name;
	char	m[MAP_HEIGHT][MAP_WIDTH];
	int		px;
	int		py;
	int		key;
	int		score;
} t_map;
```

---

### **ex07: Level Transitions**
**Goal:**  
Automatically advance to the next level (maps 1 → 4).

**Compare:**
```bash
./example07
```

<details>
<summary>💡 Hint</summary>

Track level index and reload map files accordingly.
</details>

---

### **ex08: Time Management**
**Goal:**  
- Show countdown (30 seconds to finish each map)  
- Print map every 50 ms  
- Allow movement every 200 ms  
- Reset timer to 30 seconds on new map

**Compare:**
```bash
./example08
```

**Prototype:**
```c
typedef struct s_map
{
	char	*name;
	char	m[MAP_HEIGHT][MAP_WIDTH];
	int		px;
	int		py;
	int		key;
	int		score;
	long	start_time;     // map start time (ms)
	long	last_print;     // last screen update time
	long	last_move;      // last movement accepted
} t_map;
```

<details>
<summary>💡 Hint</summary>

Use `select()` or timing functions to control frame rate and player input speed.
</details>

---

### **ex09: Game Menu**
**Goal:**  
Main menu with **Start Game** / **Quit** options.  
Highlight the selected option, confirm with Enter.

**Compare:**
```bash
./example09
```

---

### **ex10: Hidden Terminal (like vim/man)**
**Goal:**  
Use an **alternate screen buffer** for the game, restore terminal afterward, and print results in the main terminal.

**Compare:**
```bash
./example10
```

<details>
<summary>💡 Hint</summary>

```c
#define ENTER_ALT_SCREEN  "\033[?1049h"
#define EXIT_ALT_SCREEN   "\033[?1049l"
```
</details>

---

### **ex11: Audio**
**Goal:**  
Play a sound when collecting a coin.

**Compare:**
```bash
./example11
```

<details>
<summary>💡 Hint</summary>

```c
system("paplay " COIN_WAV " &");
```
</details>

---

### **ex12: Competitiveness**
**Goal:**  
Add the time left to the score at the end of each level. Find a balance that will avoid too many draws and encourage players to pick collectibles.

**Compare:**
```bash
./example12
```

---

## 🚀 Rush: Terminal-Based Game
- **Duration:** 1 week (+1 week polish)  
- **Rules:** No need to use the provided map system — can be any game concept.  
- **Goal:** Should feel like a real game.  
- **Performance:** Should run well on a **1920×1080** terminal (zoom allowed).
- **Group size:** Recommended for 2 people but can be also done solo or in a bigger group.

---

## 🛠️ Notes
- Keep terminal settings safe: always restore cursor, echo, and screen mode after exiting.  
- Test on different terminal sizes and font zoom levels.  
- Try to make your game **fun** even with the limitations.

---

## 📜 Attributions
[Coin sound](https://opengameart.org/content/plingy-coin) (by Fupi) is from OpenGameArt.Org and in CC0 license

---
