import java.util.Scanner;
import java.util.Locale;
import java.util.Arrays;

public class AC {

    public static class Pair implements Comparable<Pair> {

        private double value;
        private int position;

        Pair(double value, int position) {
            this.value = value;
            this.position = position;
        }

        double getValue() {
            return this.value;
        }

        int getPosition() {
            return this.position;
        }

        public int compareTo(Pair p)
        {
            if (getValue() < p.getValue())
                return -1;
            else if (getValue() > p.getValue())
                return 1;
            else
                return 0;
        }
    }
        
    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        Locale locale = Locale.ENGLISH;  
        scanner.useLocale(locale);  

        int N = scanner.nextInt();
        float M = scanner.nextFloat();

        Pair xs[] = new Pair[N];

        for (int i = 0; i < N; ++i)
            xs[i] = new Pair(scanner.nextDouble(), i + 1);

        Arrays.sort(xs);

        int ans[] = new int[N];
        int k = 0;

        for (int i = 0; i < N; ++i) {
            if (xs[i].getValue() <= M) {
                ans[k] = xs[i].getPosition();
                k += 1;
                M -= xs[i].getValue();
            } else {
                break;
            }
        }

        System.out.println(k);

        if (k > 0) {
            for (int i = 0; i < k; ++i) {
                if (i > 0)
                    System.out.print(" ");

                System.out.print(ans[i]);
            } 
            System.out.println();
        } 
    }

}
