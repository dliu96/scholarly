/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHER STUDENTS. Derek Liu
*/

import java.util.ArrayList;
import java.util.Random;

import javax.swing.JOptionPane;

public class GuessingGame {
	
	ArrayList<Integer> pg;
	int totalNum;
	int myGuess;
	Random myRNG = new Random();
	
	// fill in code here
	// define data members

	public GuessingGame ( ) {
		// fill in code here
		// initialization
		pg= new ArrayList<Integer>();
        for(int i =1000; i<10000; i++){
            pg.add(i);
        }
        totalNum = 0;
	}

	public int myGuessIs() {
		// fill in code here
		if (pg.size() > 0){
			myGuess = pg.get(myRNG.nextInt(pg.size()));
			totalNum++;
			return myGuess;
		} else
			return -1;
	}
	
	public int totalNumGuesses() {
		// fill in code here
		// this should return the total number of guesses taken
		return totalNum;
	}
 
	public void updateMyGuess(int nmatches) {
		// fill in code here
		// update the guess based on the number of matching digits claimed by the user
		
		String o = String(myGuess);
		char [] f = new char [4];
		f[0] = o.charAt(0);
		f[1] = o.charAt(1);
		f[2] = o.charAt(2);
		f[3] = o.charAt(3);
		int [] z = new int [4];
		for (int i = 0; i < f.length; i++){
			z[i] = ((int)(f[i])-48);
		}
		
		int count = 0;
		if (nmatches == 3) {
			for (int i = 0; i < pg.size(); i++){
				if (z[0] == (pg.get(i)) / 1000){
					count++;
				}
				if (z[1] == ((pg.get(i)) / 100)%10){
					count++;
				}
				if (z[2] == ((pg.get(i)) / 10)%10){
					count++;
				}
				if (z[3] == ((pg.get(i))%10)){
					count++;
				}
				if (count != 3){
					pg.remove(i);
					i--;
				}
				count = 0;
			}	
			
		} else if (nmatches == 2) {
			for (int i = 0; i < pg.size(); i++){
				if (z[0] == (pg.get(i)) / 1000){
					count++;
				}
				if (z[1] == ((pg.get(i)) / 100)%10){
					count++;
				}
				if (z[2] == ((pg.get(i)) / 10)%10){
					count++;
				}
				if (z[3] == ((pg.get(i))%10)){
					count++;
				}
				if (count != 2){
					pg.remove(i);
					i--;
				}
				count = 0;
			}
			
		} else if (nmatches == 1) {
			for (int i = 0; i < pg.size(); i++){
				if (z[0] == (pg.get(i)) / 1000){
					count++;
				}
				if (z[1] == ((pg.get(i)) / 100)%10){
					count++;
				}
				if (z[2] == ((pg.get(i)) / 10)%10){
					count++;
				}
				if (z[3] == ((pg.get(i))%10)){
					count++;
				}
				if (count != 1){
					pg.remove(i);
					i--;
				}
				count = 0;
			}
				
		} else if (nmatches == 0) {
			for (int i = 0; i < pg.size(); i++){
				if (z[0] == (pg.get(i)) / 1000){
					count++;
				}
				if (z[1] == ((pg.get(i)) / 100)%10){
					count++;
				}
				if (z[2] == ((pg.get(i)) / 10)%10){
					count++;
				}
				if (z[3] == ((pg.get(i))%10)){
					count++;
				}
				if (count != 0){
					pg.remove(i);
					i--;
				} 
				count = 0;
			}
		}
		
	}
	
	public String String(int i){
		String a = Integer.toString(i);
		return a;
	}
	
	
	// fill in code here (optional)
	// feel free to add more methods as needed
	
	// you shouldn't need to change the main function
	public static void main(String[] args) {

		GuessingGame gamer = new GuessingGame( );
  
		JOptionPane.showMessageDialog(null, "Think of a number between 1000 and 9999.\n Click OK when you are ready...", "Let's play a game", JOptionPane.INFORMATION_MESSAGE);
		int numMatches = 0;
		int myguess = 0;
		
		do {
			myguess = gamer.myGuessIs();
			if (myguess == -1) {
				JOptionPane.showMessageDialog(null, "I don't think your number exists.\n I could be wrong though...", "Mistake", JOptionPane.INFORMATION_MESSAGE);
				System.exit(0);
			}
			String userInput = JOptionPane.showInputDialog("I guess your number is " + myguess + ". How many digits did I guess correctly?");
			// quit if the user input nothing (such as pressed ESC)
			if (userInput == null)
				System.exit(0);
			// parse user input, pop up a warning message if the input is invalid
			try {
				numMatches = Integer.parseInt(userInput.trim());
			}
			catch(Exception exception) {
				JOptionPane.showMessageDialog(null, "Your input is invalid. Please enter a number between 0 and 4", "Warning", JOptionPane.WARNING_MESSAGE);
				numMatches = 0;
			}
			// the number of matches must be between 0 and 4
			if (numMatches < 0 || numMatches > 4) {
				JOptionPane.showMessageDialog(null, "Your input is invalid. Please enter a number between 0 and 4", "Warning", JOptionPane.WARNING_MESSAGE);
				numMatches = 0;
			}
			if (numMatches == 4)
				break;
			// update based on user input
			gamer.updateMyGuess(numMatches);
		} while (true);
		
		// the game ends when the user says all 4 digits are correct
		System.out.println("Aha, I got it, your number is " + myguess + ".");
		System.out.println("I did it in " + gamer.totalNumGuesses() + " turns.");
	}
}
