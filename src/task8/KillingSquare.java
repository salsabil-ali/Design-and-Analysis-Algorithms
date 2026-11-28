package task8;


import java.util.Arrays;

public class KillingSquare {

    private int n;
    private int[] masks;
    private int[] dp;
    private int[] choice;
    private int FULL;

    public int[][] breakAllSquares(int n) {

        this.n = n;

        int T = 2 * n * (n + 1);
        int S = n * (n + 1) * (2 * n + 1) / 6;

        FULL = (1 << S) - 1;

        masks = new int[T];
        dp = new int[1 << S];
        choice = new int[1 << S];

        Arrays.fill(dp, -1);

        // Calculate which squares each toothpick breaks
        int s = 0;

        for (int k = 1; k <= n; k++) {
            for (int r = 0; r <= n - k; r++) {
                for (int c = 0; c <= n - k; c++) {

                    for (int j = 0; j < k; j++) {

                        // Top and bottom boundaries
                        masks[r * n + c + j] |= (1 << s);
                        masks[(r + k) * n + c + j] |= (1 << s);

                        // Left and right boundaries
                        masks[n * (n + 1) + (r + j) * (n + 1) + c]
                                |= (1 << s);

                        masks[n * (n + 1) + (r + j) * (n + 1) + c + k]
                                |= (1 << s);
                    }

                    s++;
                }
            }
        }

        // Find minimum number of removals
        int minimum = solve(0);

        // Reconstruct the optimal solution
        int[][] removedToothpicks = new int[minimum][2];

        int mask = 0;

        for (int i = 0; i < minimum; i++) {
            int t = choice[mask];

            removedToothpicks[i] = getCoordinates(t);
            mask |= masks[t];
        }

        return removedToothpicks;
    }

    private int solve(int mask) {

        if (mask == FULL) {
            return 0;
        }

        if (dp[mask] != -1) {
            return dp[mask];
        }

        int minimum = Integer.MAX_VALUE;

        for (int t = 0; t < masks.length; t++) {

            int newMask = mask | masks[t];

            if (newMask == mask) {
                continue;
            }

            int cost = 1 + solve(newMask);

            if (cost < minimum) {
                minimum = cost;
                choice[mask] = t;
            }
        }

        dp[mask] = minimum;

        return minimum;
    }

    private int[] getCoordinates(int t) {

        int horizontalCount = n * (n + 1);

        if (t < horizontalCount) {
            return new int[]{
                2 * (t / n),
                2 * (t % n) + 1
            };
        }

        t -= horizontalCount;

        return new int[]{
            2 * (t / (n + 1)) + 1,
            2 * (t % (n + 1))
        };
    }

}