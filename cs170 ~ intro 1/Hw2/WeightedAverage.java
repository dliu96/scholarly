import java.util.Scanner;

public class WeightedAverage {
	public static void main(String[] args){
		Scanner input = new Scanner(System.in);

		System.out.println("Enter your average quiz score out of 15: ");
		double quiz = input.nextDouble();
		if (quiz < 0 || quiz > 15){
		System.out.println("This is not a valid quiz score, please try again.");
			while (quiz < 0 || quiz > 15){
			System.out.println("Enter your average quiz score out of 15: ");
			quiz = input.nextDouble();
				if (quiz >= 0 || quiz < 15){
				continue;
				}
			}
		}

		System.out.println("Enter your average homework score out of 100: ");
		double homework = input.nextDouble();
		if (homework < 0 || homework > 100){
		System.out.println("This is not a valid homework score, please try again.");
			while (homework < 0 || homework >100){
			System.out.println("Enter your average homework score out of 100: ");
			homework = input.nextDouble();
				if (homework >= 0 || homework < 100){
				continue;
				}
			}
		} 

		System.out.println("Enter your average average lab score out of 30: ");
		double lab = input.nextDouble();
		if (lab < 0 || lab > 30) {
		System.out.println("This is not a vaild lab score, please try again.");
			while (lab<0 || lab >30){
			System.out.println("Enter your average average lab score out of 30: ");
			lab = input.nextDouble();
				if (lab >= 0 || lab < 30){
				continue;
				}
			}
		}

		System.out.println("Enter your average midterm exam score out of 100: ");
		double midterm = input.nextDouble();
		if (midterm < 0 || midterm > 100){
		System.out.println("This is not a valid midterm score, please try again.");
			while (midterm <0 || midterm >100){
			System.out.println("Enter your average midterm exam score out of 100: ");
			midterm = input.nextDouble();
				if (midterm >= 0 || midterm < 15){
				continue;
				}
			}
		}

		System.out.println("Enter your final exam score out of 100: ");
		double finalexam = input.nextDouble();
		if (finalexam < 0 || finalexam > 100){
		System.out.println("This is not a valid final exam score, please try again.");
			while (finalexam < 0 || finalexam > 100){
			System.out.println("Enter your final exam score out of 100: ");
			finalexam = input.nextDouble();
				if (finalexam >= 0 || finalexam < 15){
				continue;
				}
			}
		}
		
		double weightedscore = (25 * (homework/100)) + (10 * (lab/30)) + (10 * (quiz/15)) + (30 * (midterm/100)) + (25 * (finalexam/100));
		System.out.println("Your weighted average in the class is: "+ weightedscore + ".");
											
	}
}
	
