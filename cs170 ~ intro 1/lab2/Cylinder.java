import java.util.Scanner;
    
  /* This program calculates the volume and surface area of a cylinder */
   
  public class Cylinder   {
     public static void main(String [] args) {
   
	// declaration of the variables
      	double radius, height, volume, surfacearea;

	// create a Scanner
    	Scanner input;
	input = new Scanner(System.in);

	// Enter radius   
	System.out.print("Please enter radius: ");
	   
	// Read input from terminal and store it in the appropriate variable    
      	radius = input.nextDouble();

	// Enter height
	System.out.print("Please enter height: ");

	// Read input from terminal and store it in the appropriate variable
	height = input.nextDouble();

	// define a variable for pi
     	final double PI = 3.1415926535;

	//compute the volume of the cylinder and store it in appropriate variable
	volume = PI * radius * radius * height;

	//computer the surface area and store it in appropriate variable.
      	surfacearea = (2 * PI * radius * height) + (2 * PI * radius * radius);

	// print out the result
	System.out.println("For a cylinder with a radius of " + radius + " and a height of " + height + " the volume is " + volume + " and the surface area is " + surfacearea + ".");
      } 
}
