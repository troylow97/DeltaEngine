# DeltaEngine

Generate the solution by running the "GenerateProjects.bat"

Build Type:
Debug - Meant for Game, currently empty
Debug Editor
Release - Meant for Game, currently empty
Release Editor

Keyboard Shortcuts:

Ctrl + N - New Scenes
Ctrl + O - Open Scenes
Ctrl + S - Save Scenes

Ctrl + Shift + A - Add New Entity
Ctrl + Shift + V - Clone Entity (clone current selected entity)
Ctrl + Shift + C - Clone Entity Default (clone current selected entity with default values)
Del - Delete Entity (destroy currently selected Entity)

Ctrl + W - Open/Close World Panel
Ctrl + I - Open/Close Inspector Panel
Ctrl + T - Open/Close Tile Panel

Ctrl + Return - Prints performance information into Console

Current keyboard controls for Game:
Left / Right Arrow Key - Move player left / right
Spacebar - Jump
C - Ranged attack (no animation see with Gizmo on)
X - Melee attack
\ - Turn on Gizmo Draw

Functionality:
Asset Panel (scans folder and show all assets)
1. Select a Folder
2. Drag a file into the respective inspector properties
  - Currently only works for textures & animation, TBC Text & Shaders
  - Select a .png texture file and drag it to the image sprite property
  - Select a .anim animation file and drag it to the animator controller property
3. Filter for files

Tilemap Panel (scans tilemap folder)
1. Drag a tile onto the world
2. Modify the new entity as required for the game

World Panel
1. List all entities and their assigned name

Property Inspector Panel
1. Add component through the drop-down list
2. Remove component by pressing on the cross

Editor Camera
1. Modify the position, size, rotation

Play / Pause
1. Play/Pause will enable/disable all the logic system and physics

Default Entity is created with 
- Transform
- Name

Components required for physics system:
- Transform
- Rigidbody
- Collider

Components required for object with textures:
- Transform
- Image
- Renderer2D

Components required for object with text:
- Transform
- Text
- Renderer2D

Components required for animation:
- Transform
- Image
- Renderer2D
- Animator
- State