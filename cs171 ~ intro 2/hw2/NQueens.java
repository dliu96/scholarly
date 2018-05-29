/*
 THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
 CODE WRITTEN BY OTHER STUDENTS. Derek Liu
 */

import java.util.Stack;

public class NQueens {

	// ***** fill in your code here *****
	// feel free to add additional methods as necessary

	public static boolean canPlace(int x, Stack<Integer> chess) {
		for (int i = 0; i < chess.size(); i++) {
			if (chess.get(i) == x) {
				return false;
			} else if (chess.size() - i == Math.abs(x - chess.get(i))) {
				return false;
			}
		}
		return true;
	}

	// finds and prints out all solutions to the n-queens problem
	public static int solve(int n) {

		// ***** fill in your code here *****
		Stack<Integer> stack = new Stack<Integer>();
		int count = 0;
		int x = 0;

		stack.push(0);

		do {
			if (n == x) {
				// x = queenNum.pop() + 1;
				if (stack.isEmpty()) {
					break;
				}
				x = stack.pop() + 1;
			} else if (n > x && canPlace(x, stack)) {
				stack.push(x);
				x = 0;
			} else {
				x++;
			}

			if (stack.size() == n) {
				printSolution(stack);
				count++;
				x = stack.pop() + 1;
			}
		} while (true);

		// update the following statement to return the number of solutions
		// found
		return count;

	}// solve()

	// this method prints out a solution from the current stack
	// (you should not need to modify this method)
	private static void printSolution(Stack<Integer> s) {
		for (int i = 0; i < s.size(); i++) {
			for (int j = 0; j < s.size(); j++) {
				if (j == s.get(i))
					System.out.print("Q ");
				else
					System.out.print("* ");
			}// for
			System.out.println();
		}// for
		System.out.println();
	}// printSolution()

	// ----- the main method -----
	// (you shouldn't need to change this method)
	public static void main(String[] args) {

		int n = 8;

		// pass in parameter n from command line
		if (args.length == 1) {
			n = Integer.parseInt(args[0].trim());
			if (n < 1) {
				System.out.println("Incorrect parameter");
				System.exit(-1);
			}// if
		}// if

		int number = solve(n);
		System.out.println("There are " + number + " solutions to the " + n
				+ "-queens problem.");
	}// main()

}
