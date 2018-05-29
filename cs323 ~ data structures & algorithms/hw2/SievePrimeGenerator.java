import java.util.Scanner;
class SievePrimeGenerator {
	static int array[];
	static int primes[];

	public static void segmentedSieve(int lower, int upper) {
		//how we will keep track of our primes
		int array[];
		int primes[];

		int j = 0;
		int sqt = (int) Math.sqrt(upper);

		//initialize our arrays
		array = new int[sqt + 1];
		primes = new int[sqt + 1];
 		//System.out.println("sqt:" + sqt);
		
		//initialise(sqt + 1);
		for (int i = 0; i < sqt + 1; i++) {
			//set every number to 1 to indicate the possibility of prime
			array[i] = 1;
		}

		//the sieve up to the sqt(upper)
		for (int i = 2; i <= sqt; i++) {
			//we have a prime
			if (array[i] == 1) {
				primes[j] = i;
				j++;

				//mark multiples of our prime off
				for (int k = i + i; k <= sqt; k += i) {
					array[k] = 0;
				}
			}
		}

		//get an idea for how many primes we're looking for
		int diff = (upper - lower + 1);
		array = new int[diff];
		
		//initialise(diff);
		for (int i = 0; i < diff; i++) {
			//set every number to 1 to indicate the possibility of prime
			array[i] = 1;
		}

		//the segmented sieve algorithm
		int k = 0;
		while (k < j) {
			int div = lower / primes[k];
			div *= primes[k];
			while (div <= upper) {
				if(div>=lower && primes[k]!=div)
					array[div-lower] = 0;
				div += primes[k];
			}

			k++;
		}

		for (int i = 0; i < diff; i++) {
			if (array[i] == 1 && (i+lower) !=1)
				System.out.println(i + lower);
		}
	}


	public static void main(String arg[]) {
		int n, m;
		Scanner in = new Scanner(System.in);
		//t = in.nextInt();
		//for (int i = 0; i < t; i++) {
			n = in.nextInt();
			m = in.nextInt();
			segmentedSieve(n, m);
			System.out.println();
		//}
		in.close();
	}
}