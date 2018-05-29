import java.util.*;

public class MyTestMap
{
   public static void main(String[] args) 
   {
	   
      int i;
      Integer x;
      String s;

      Map<String, Integer> M = new HashListMap<String, Integer>(4);
      

      M.put("a", 1); System.out.println("+(a,1): " + M);
      M.put("b", 5); System.out.println("+(b,5): " + M);
      M.put("c", 6); System.out.println("+(c,6): " + M);
      M.put("d", 8); System.out.println("+(d,8): " + M);
      M.put("d", 3); System.out.println("*(d,3): " + M);
      M.put("e", 4); System.out.println("+(e,4): " + M);
      M.put("e", 7); System.out.println("*(e,7): " + M);
      M.put("f", 9); System.out.println("+(f,9): " + M);
      M.put("g", 2); System.out.println("+(g,2): " + M);
      System.out.println();
      
//      M.put("z", 1); System.out.println("+(z,1): " + M);
//      M.put("w", 5); System.out.println("+(w,5): " + M);
//      M.put("q", 6); System.out.println("+(q,6): " + M);
//      M.put("q", 8); System.out.println("*(q,8): " + M);
//      M.put("r", 3); System.out.println("+(r,3): " + M);
//      M.put("robby", 4); System.out.println("+(robby,4): " + M);
//      M.put("antonio", 7); System.out.println("+(antonio,7): " + M);
//      M.put("m", 9); System.out.println("+(m,9): " + M);
//      M.put("antonio", 2); System.out.println("+(antonio,2): " + M);
//      System.out.println();

      x = M.get("d"); System.out.println("Value of d = "+((x==null)?"null":x));
      x = M.get("e"); System.out.println("Value of e = "+((x==null)?"null":x));
      x = M.get("x"); System.out.println("Value of x = "+((x==null)?"null":x));
      System.out.println();

      M.remove("a"); System.out.println("-a: " + M);
      M.remove("a"); System.out.println("-a: " + M);
      M.remove("b"); System.out.println("-b: " + M);
      M.remove("b"); System.out.println("-b: " + M);
      M.remove("c"); System.out.println("-c: " + M);
      M.remove("d"); System.out.println("-d: " + M);
      M.remove("g"); System.out.println("-g: " + M);
   }
}