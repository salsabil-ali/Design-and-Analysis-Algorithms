package task8;

import java.util.Arrays;

public class Main {
    
    public static void main(String[] args) {

        KillingSquare ks = new KillingSquare();

        int[][] result = ks.breakAllSquares(2);

        System.out.println("Minimum removals: " + result.length);

        for (int[] t : result) {
            System.out.println(Arrays.toString(t));
        }
    }
}
