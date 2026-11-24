# Task 7 - Poisoned Wine (Divide and Conquer)

`designWineTests(testers)` returns `(possible, schedule, decoder)` for 1000 barrels, 2 tasting days (day 0 and day 5), so k testers separate 3^k barrels.
The barrel range is split in 3 parts; tester k-1 skips part 0, drinks part 1 on day 0 and part 2 on day 5; the other k-1 testers recurse inside each part.

## Run

```
g++ -std=c++11 task7_poisoned_wine.cpp -o wine

Input: one or more tester counts (`10 8 7 6`). Output: possible?, verification of all 1000 barrels, schedule sizes, sample decodes.

Files: `task7_poisoned_wine.cpp`
```
