import java.util.*;

/**
 * Task 4 - Point Coloring (Iterative Improvement)
 *
 * Pair the points on every line (1st-2nd, 3rd-4th, ...). If every pair has two
 * different colors, every line is balanced. Start with all points white, then
 * repeatedly fix a bad pair (same colors) by flipping a chain of points.
 */
public class PointColoring {

    static final int VERTICAL = 0;
    static final int HORIZONTAL = 1;

    static int[][] readPoints(Scanner sc) {
        System.out.print("How many points? ");
        int n = sc.nextInt();
        int[][] points = new int[n][2];
        for (int i = 0; i < n; i++) {
            System.out.print("Point " + (i + 1) + " (x y): ");
            points[i][0] = sc.nextInt();
            points[i][1] = sc.nextInt();
        }
        return points;
    }

    // key = coordinate defining the line, other = coordinate used for sorting
    static void buildPairs(int[][] p, int n, int key, int other, int[] partner) {
        Integer[] idx = new Integer[n];
        for (int i = 0; i < n; i++) idx[i] = i;
        Arrays.sort(idx, (a, b) -> p[a][key] != p[b][key]
                ? Integer.compare(p[a][key], p[b][key])
                : Integer.compare(p[a][other], p[b][other]));

        int pos = 0;
        for (int i = 0; i < n; i++) {
            if (i > 0 && p[idx[i]][key] == p[idx[i - 1]][key]) pos++;
            else pos = 0;
            if (pos % 2 == 1) {
                partner[idx[i]] = idx[i - 1];
                partner[idx[i - 1]] = idx[i];
            }
        }
    }

    static char[] colorPoints(int n, int[][] points) {
        int[][] partner = new int[2][n];
        Arrays.fill(partner[VERTICAL], -1);
        Arrays.fill(partner[HORIZONTAL], -1);
        buildPairs(points, n, 0, 1, partner[VERTICAL]);
        buildPairs(points, n, 1, 0, partner[HORIZONTAL]);

        boolean[] black = new boolean[n];

        boolean improved = true;
        while (improved) {
            improved = false;
            for (int u = 0; u < n; u++) {
                for (int dir = 0; dir < 2; dir++) {
                    int v = partner[dir][u];
                    if (v != -1 && black[u] == black[v]) {
                        fixBadPair(v, 1 - dir, partner, black);
                        improved = true;
                    }
                }
            }
        }

        char[] colors = new char[n];
        for (int i = 0; i < n; i++) colors[i] = black[i] ? 'B' : 'W';
        return colors;
    }

    // Flip v, then continue along the chain until it ends or the next bad pair is fixed
    static void fixBadPair(int v, int nextDir, int[][] partner, boolean[] black) {
        int cur = v;
        while (true) {
            int next = partner[nextDir][cur];
            boolean stop = (next == -1) || (black[cur] == black[next]);
            black[cur] = !black[cur];
            if (stop) break;
            cur = next;
            nextDir = 1 - nextDir;
        }
    }

    static boolean isValid(int n, int[][] p, char[] c) {
        Map<Integer, int[]> xs = new HashMap<>(), ys = new HashMap<>();
        for (int i = 0; i < n; i++) {
            int[] a = xs.computeIfAbsent(p[i][0], k -> new int[2]);
            int[] b = ys.computeIfAbsent(p[i][1], k -> new int[2]);
            int col = c[i] == 'B' ? 0 : 1;
            a[col]++;
            b[col]++;
        }
        for (int[] a : xs.values()) if (Math.abs(a[0] - a[1]) > 1) return false;
        for (int[] b : ys.values()) if (Math.abs(b[0] - b[1]) > 1) return false;
        return true;
    }

    static void solveAndPrint(int[][] points) {
        char[] res = colorPoints(points.length, points);
        for (int i = 0; i < points.length; i++)
            System.out.println("(" + points[i][0] + "," + points[i][1] + ") -> " + res[i]);
        System.out.println("Colors: " + new String(res));
        System.out.println("Valid : " + isValid(points.length, points, res));
        System.out.println();
    }

    static void runTests() {
        int[][][] tests = {
            {{0,0},{0,1},{1,0},{1,1}},
            {{0,0},{0,1},{0,2},{0,3},{0,4}},
            {{0,0},{0,1},{1,1},{1,2},{2,2},{2,0}},
            {{0,0},{1,0},{2,0},{1,1},{1,2}}
        };
        for (int t = 0; t < tests.length; t++) {
            System.out.println("Test " + (t + 1));
            solveAndPrint(tests[t]);
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("1) Enter my own points");
        System.out.println("2) Run the test cases");
        System.out.print("Choice: ");
        int choice = sc.nextInt();

        if (choice == 1) solveAndPrint(readPoints(sc));
        else runTests();
    }
}
