
Implemented features from the task document. C++, server authoritative, GAS throughout. Built in Unreal Engine 5.7

---

## Getting started (Visual Studio & Rider)

1. Clone the repo:
```bash
   git clone https://github.com/Rithvik-gamedev/AVTask.git
```
2. Right-click the `.uproject` → **Generate Visual Studio project files**. On Rider, just
   right-click the `.uproject` and open it directly — Rider picks up the rest.
3. Build:
   - VS 2022: Open the `.sln`, set config to `Development Editor | Win64`, hit `Ctrl+Shift+B`.
   - Rider: Open the project, let it sync with UE, then `Ctrl+B`.
4. Open the `.uproject` in UE 5.7 and hit Play.

## Controls

The project uses the Enhanced Input System. Default bindings:

| Action | Input |
|--------|-------|
| Move | W A S D |
| Look | Mouse |
| Jump | Space |
| Attack | Left Mouse Button |
| Dash | Left Shift / Right Mouse Button |

## Part 1 - Core character & combat

### Character attributes

Health, MaxHealth, Stamina, AttackPower, Defense, and MoveSpeed are in. Health and Stamina
replicate to clients. Death triggers at zero health through the shared damage pipeline. HUD shows
both. Input uses the Enhanced Input System with data driven bindings.

### Gameplay framework

GameMode handles default pawn setup and respawn. The ASC lives in the PlayerState - keeps it alive
across respawns and handles replication correctly.

### Ability system

- Sword equips to a hand socket, activates via GAS, and drains Stamina per attack.
- Death is a gameplay ability. Handles the state transition and disables gameplay actions for
  both player and AI.

### Skills

Dash is a GAS mobility ability - consumes Stamina and pauses regeneration while active.
Unlocking via level progression isn't implemented.

### Animations - known issue

Attack animations are from Mixamo. The rig doesn't cooperate with Layered Blend Per Bone, so
lower body locomotion cuts out during attacks rather than blending underneath. Still unresolved.

---

## Part 2 - World systems

### Collectables & potions

Health and Stamina potions are in. Overlap detection runs on the server. Potion pickups apply
Gameplay Effects through the shared attribute system.

---

## AI enemy system

Enemies have their own Attribute Component and combat abilities. Behavior Trees handle detection,
chasing (run speed), and patrolling (walk speed). AI attacks share the same damage pipeline as
players. Death disables targeting and combat.

---

## Interaction systems

### Movable platform

Spline driven movement, server side. Position replicates to clients.

---

## Multiplayer & architecture

Server authoritative throughout. ASC replication and state recovery hold up after respawn. C++
systems expose tunable parameters to Blueprint.

---

*Systems in the task document but not Implemented: Level Progression, Inventory, Equipment Slots, Parry, Checkpoints,
Final Win Gate, Save System*