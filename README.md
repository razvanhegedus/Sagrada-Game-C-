# 🟦 Sagrada: C Game Implementation

This project is a simplified implementation of the **Sagrada** board game written in C. Sagrada is a competitive dice-drafting game where players build beautiful stained glass windows while meeting complex placement restrictions and scoring objectives.

## 🎮 Game Summary

Players take turns drafting and placing colored dice on a personal grid (their stained glass window), adhering to constraints on die color, value, and placement. The game spans 10 rounds and includes both public and private scoring objectives.

## 🧱 Components Modeled

This implementation includes data structures and logic for:

- 🎲 **Dice** – with `color` and `value`.
- 🃏 **Objective Cards**
  - **Public Objectives** (shared goals)
  - **Private Objectives** (secret individual goals)
- 📐 **Window Pattern Cards** – with color/value placement constraints.
- 🧑 **Player State** – tracks player ID, window grid, objectives, and score.
- 📊 **Game State** – maintains rounds, player turns, dice bag, draft pool, and round track.

## ⚙️ File Overview

- `implementation.c`: Core game logic and structures implemented in C.
- 'matrix.txt' : Contains the board configuration, each player wil be assigned one of the board in the file
- 'publicgoals.txt' : Stores the public goals.

## 🕹️ Gameplay Mechanics

- **Rounds:** 10 rounds total, with clockwise and reverse-order dice drafts.
- **Placement Rules:** Dice must:
  - Match window color/value restrictions
  - Be placed orthogonally adjacent to existing dice
  - Avoid placing same color/value orthogonally
- **Scoring:**
  - Points from public and private objectives
  - Bonus points for unused favor tokens
  - Penalties for empty spaces

## ❌ Simplifications

- Tool cards are defined in the design doc but **not implemented** in this version.

## 🛠️ Compilation & Running

To compile the game:

```bash
gcc implementation.c -o sagrada
