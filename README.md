# Task 1 - Coin Removal | EED461 Design and Analysis of Algorithms

**Student Name:** Salsabil Ali Mahmoud  
**Student ID:** 2300127 (Bylaw: UG2023)  
**Required Technique:** Greedy  

---

## 📌 Problem Description
A line of coins lies on a table, each showing either heads (`H`) or tails (`T`). A move removes any heads-up coin and then flips its immediate neighbors (if present). Removing a coin does not close the gap. The goal is to determine precisely which starting rows can be cleared and provide a valid removal sequence.

---

## 💡 Algorithmic Approach (Greedy)
1. **Parity Requirements:** 
   - Heads (`H`) requires an even number of neighbor removals (0 or 2).
   - Tails (`T`) requires an odd number of neighbor removals (exactly 1).
2. **Greedy Pass:** We iterate from left to right, fixing the orientation of edges and matching the required parities.
3. **Topological Sort:** We construct a directed path based on the removal dependencies and use a queue to extract the final topological order of removals.

---

## ⏱️ Complexity Analysis
- **Time Complexity:** $\mathcal{O}(n)$ — due to a single linear greedy pass combined with topological sorting.
- **Space Complexity:** $\mathcal{O}(n)$ — for storing the sequence arrays, in-degree counts, and queue structures.

---

## 🚀 How to Run & Test
1. Make sure you have Python installed.
2. Run the script using individual test cases:
   ```bash
   python task1_coin_removal.py HTHHT
   ```
3. Run using the provided test input file:
   ```bash
   python task1_coin_removal.py -f task1_inputs.txt
   ```
4. Run the exhaustive automated test suite ($n = 1$ to $16$):
   ```bash
   python test_task1.py
   ```
# CSE461: Design and Analysis of Algorithms - Project (2026/2027)

**University:** Ain Shams University  
**Faculty:** Faculty of Engineering  
**Program:** Computer Engineering and Software Systems Program  

---

## 👥 Team Members & Student Information
* **Salsabil Ali Mahmoud** — ID: 2300127 (Bylaw: UG2023)

---

## 🎥 Presentation & Demo Video
* **Recorded Presentation & Demo Link:** [Insert YouTube / Google Drive Link Here] *(Maximum duration: 20 minutes)*

---

## 📂 Project Tasks Structure
This repository contains all the tasks implemented using their required algorithmic techniques:

| Task | Topic | Required Technique | Status |
| :--- | :--- | :--- | :--- |
| **Task 1** | Coin Removal | Greedy | Completed |
| **Task 2** | Domino Tiling | Brute Force | Pending |
| **Task 3** | Security Switches | Decrease and Conquer | Pending |
| **Task 4** | Point Coloring | Iterative Improvement | Pending |
| **Task 5** | King Arthur's Round Table | Dynamic Programming | Pending |
| **Task 6** | Killing Squares | Dynamic Programming | Pending |
| **Task 7** | Poisoned Wine | Divide and Conquer | Pending |
| **Task 8** | Colored Tromino Tiling | Divide and Conquer | Pending |

---

## 🛠️ Repository Navigation
* Explore each task inside its dedicated folder (e.g., `Task1 Coin Removal/`).
* Each folder includes source code, test inputs, validation scripts, and a detailed explanation PDF.
