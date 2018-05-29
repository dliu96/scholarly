public class Triangle1 {
	public static void indentCharLine(int i, char c, int n) {
		String out = "";

		//build up indentation using spaces
		for(int x = 0; x < i; x++) {
			out += " ";
		}

		//build up characters
		for(int x = 0; x < n; x++) {
			out += c;
		}
	
		//print out the new string
		System.out.println(out);
   	}
	
	public static void main(String args[]) {
      /* -----------------------------------------
         Make sure user provides 1 argument
         ----------------------------------------- */
      if ( args.length != 1 ) {
         System.err.println("Error: Program needs 1 integer as input");
         System.exit(1);
      }
      int i;

      /* -----------------------------------------
         Convert first argument to integer
         ----------------------------------------- */
      int n = Integer.parseInt( args[0] );
	
	  /* **************************************************
         +---------+
         |  TODO:  |
         +---------+

         Use a for-loop to print the following triangle:

         X          ^
         XX         |
         ...        |  n rows
         XXXX       |
         XXXXX      |
         XXXXXX     v
         <---->
         base = n

         ++++ Note: the body of the for-loop uses the method
                    indentCharLine() !!!
         ************************************************** */ 
	for ( i = 0; i < n; i++){
		indentCharLine (0, 'X', i+1);
		}
	}
}
