# Task 3 - Security Switches (Decrease and Conquer

import json
import os

# Part 1: To check if the list of moves is legal

def is_valid(n, toggles):
        on = [True] * (n + 1) # "on" is a list that remembers whether each switch is on (True) or off (False) and it has n+1 slots
        for p in toggles: # go through the moves one by one
                if p < 1 or p > n:
                        return False # A switch number below 1 or above n doesn't exist so sequence is invalid
                if p < n:
                        if not on[p + 1]: # the neighbor on the right (p + 1) must be ON
                                return False
                        if any(on[p + 2 : n + 1]): # all switches farther to the right (p+2 up to n) must be OFF
                                return False
                on[p] = not on[p] # move was legal therefore flip the switch
        return not any(on[1:]) # After all the moves, the answer is valid only if every switch is OFF

# Part 2: The decrease and conquer solution

def generate_off(m): # returns the moves that turn off the m rightmost switches starting from all of them being ON
        if m == 0: # in case of empty list
                return []
        if m == 1: # in case of one switch
                return [1]
        first = generate_off(m - 2)
        return first + [m] + first[::-1] + generate_off(m - 1)
def turnOffSwitches(n):
        right_numbered = generate_off(n)  # Get the moves using the "numbered from right" system
        min_moves = len(right_numbered) # length of the list is the number of moves
        toggles = [n - k + 1 for k in right_numbered]
        return (min_moves, toggles)

# Part 3: Running tests and saving results

if __name__ == "__main__":
        folder = os.path.dirname(os.path.abspath(__file__))
        with open(os.path.join(folder, "tests.json"), "r", encoding="utf-8") as f:
                tests = json.load(f)
                lines = []
                for number, test in enumerate(tests, 1):
                    n = test["n"]
                    min_moves, toggles = turnOffSwitches(n) # to run our solution
                    ok = is_valid(n, toggles) # to check if move is legal
                    lines.append(f"Test {number}: n = {n}")
                    lines.append(f"  Minimum moves: {min_moves}")
                    lines.append(f"  Toggles: {toggles}")
                    lines.append(f"  Valid sequence: {ok}")
                    lines.append("")
                text = "\n".join(lines)
                print(text)
                with open(os.path.join(folder, "output.txt"), "w", encoding="utf-8") as f:
                    f.write(text)
