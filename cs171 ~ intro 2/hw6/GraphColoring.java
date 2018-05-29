import java.util.Random;

/*
THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING 
CODE WRITTEN BY OTHER STUDENTS. Derek Liu.
*/

// Students should edit this file for homework.

// NOTE: to compile and run this, put graph.jar on your CLASSPATH
// (or unpack it in the same directory as this file).  You are welcome
// to use the classes in graph.jar, and anything from java.util.
// You should not use multiple threads.

// As usual: add honor statement up here, and fix the TODO's below.
// If you use documents or websites (beyond our textbook) for ideas,
// you should list those here.  Your code should be your own.

// The only methods you must edit are the constructor and tryImprove,
// but you are welcome to redesign other parts of the class too, if
// you like.  When we test your code on Gimle, we will use a top-level
// loop that resembles main (but maybe with more testing and output).

// A GraphColoring object represents a vertex coloring for a Graph: it
// specifies a positive integer color for each vertex v, so that
// adjacent vertices get distinct colors.  We say it is a "K coloring"
// if the maximum color used is K.  We prefer colorings with K as
// small as possible, but "best" colorings can be hard to find.
//
// This class defines two ways to define the coloring.  First, the
// constructor computes a quick initial coloring, by some fast method.
// Second, the tryImprove(secs) method can try harder to improve the
// current coloring, but it should stay within the given time limit.
//
// See main() for the command-line usage of this program.  In
// particular, it shows how to call tryImprove in a loop, but within
// some global time limit.

// Note: finding a graph coloring that uses the minimum number of
// colors is a very hard problem ("NP-hard", the worst case time is
// apparently exponential in V).  So we pursue "heuristic" solutions
// here.  That is, we try to do the best we can with the time we have
// available.  Whatever we do, we always produce a valid coloring,
// and we prefer colorings that use fewer colors.

public class GraphColoring {
	// Internal data for our coloring object.
	private Graph G; // the Graph (from the constructor)
	private int[] color; // color[v] is color of vertex v
	private boolean[] colored;
	private boolean bi = true;

	// private int maxColor; // maximum color used (K)

	// Accessor methods:
	public Graph graph() {
		return G;
	}

	public int color(int v) {
		return color[v];
	}

	public int maxColor() {
		// If you prefer, you could maintain the "maxColor" class
		// variable, and redefine this method to return its value.
		int max = 0;
		for (int c : color)
			if (c > max)
				max = c;
		return max;
	}

	// Constructor: given a graph G, build some fast initial coloring.

	public GraphColoring(Graph G) {
		this.G = G;
		// TODO: if G is bipartite, then you should find a 2-coloring
		// here in the constructor.
		if (isBipartite(G)) {
			this.color = color;
		} else {
			this.color = greedyColoring(G);
		}
	}

	public boolean isBipartite(Graph G) {
		int N = G.V();
		colored = new boolean[N];
		color = new int[N];
		for (int i = 0; i < N; i++) {
			color[i] = 1;
		}
		for (int j = 0; j < N; j++) {
			if (!colored[j])
				colorGraph(G, j);
		}
		return bi;
	}

	public void colorGraph(Graph G, int v) {
		colored[v] = true;
		for (int x : G.adj(v)) {
			if (!colored[x]) {
				if (color[v] == 1) {
					color[x] = 2;
				} else {
					color[x] = 1;
				}
				colorGraph(G, x);
			} else if (color[x] == color[v]) {
				bi = false;
			}
		}
	}

	// The greedy coloring heuristic. You may replace this with something
	// better, if you want.
	private static int[] greedyColoring(Graph G) {
		int V = G.V();
		assert V >= 1;
		// This will be our coloring array.
		int[] color = new int[V];
		// In loop, we keep track of the maximum color used so far.
		int maxColor = 0;
		// For each vertex v, we let color[v] be the first color which
		// is not already taken by the neighbors of v.
		for (int v = 0; v < V; ++v) {
			boolean[] taken = new boolean[maxColor + 1];
			for (int u : G.adj(v))
				taken[color[u]] = true;
			// Find the first color c not taken by neighbors of v.
			int c = 1;
			while (c <= maxColor && taken[c])
				++c;
			color[v] = c;
			// Maybe we started using a new color at v.
			if (c > maxColor)
				maxColor = c;
		}

		// All done, return the array.
		return color;
	}

	// Method bugs checks whether the current coloring is valid, just
	// using its public interface. It returns the number of problems
	// found. If it returns 0, then the coloring is a valid.
	public int bugs() {
		Graph G = graph();
		int V = G.V();
		int K = maxColor();
		int bugCount = 0;
		for (int v = 0; v < V; ++v) {
			int c = color(v);
			if (c < 1)
				++bugCount;
			if (c > K)
				++bugCount;
			for (int u : G.adj(v))
				if (u != v && c == color(u))
					++bugCount;
		}
		return bugCount;
	}

	// toString() lets us print a coloring. The coloring is printed
	// as two lines of text, all integer values:
	//
	// V K
	// C[0] C[1] ... C[V-1]
	//
	// Here V is the number of graph vertices, K is the maximum color
	// used, and each C[i] is the color of vertex i. The color should
	// be an integer between 1 and K, inclusive.
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(color.length).append(' ').append(maxColor()).append('\n');
		String sep = "";
		for (int c : color) {
			sb.append(sep).append(c);
			sep = " ";
		}
		sb.append('\n');
		return sb.toString();
	}

	// Method tryImprove(secs) is where we may implement some more
	// time-consuming graph coloring heuristic, which tries to improve
	// the current coloring. The boolean return value indicates
	// whether we succeeded in improving the coloring (that is,
	// reducing the number of colors used). The "secs" argument is a
	// time limit, in seconds.
	//
	// In general, we should try our heuristic for a while, trying to
	// improve the current coloring. If we succeed in finding a
	// better coloring, we should return true (then main will report
	// the coloring). If we want to quit (maybe we are out of time or
	// ideas, or the coloring is already best possible), we should
	// return false. Note that if we exceed the time limit, then
	// there is a risk that our output will be ignored: our program
	// may be killed externally!

	public boolean tryImprove(double secs) {
		// TODO: implement some graph coloring heuristic here.
		// Minimal good enough: do repeated trials of greedy-coloring,
		// each time with a random vertex order. If you find a better
		// coloring (better than your previous best), return true. Or
		// else if you run out of time, return false.

		// Note you can use System.currentTimeMillis() to estimate how
		// much time you have used. For an example of its use, see
		// the code in main.

		int N = this.G.V();
		int[] vertices = new int[N];
		double startTime = System.currentTimeMillis();
		for (int i = 0; i < N; i++) {
			vertices[i] = i;
		}

		do {
			double newTime = (System.currentTimeMillis() - startTime) / 1000;
			if (newTime >= secs) {
				return false;
			}
			shuffle(vertices);
		} while (maxVert(vertices) == false);
		return true;
	}

	private void shuffle(int[] vertices) {
		// TODO shuffle the vertices to change change the value that the
		// tryImprove(secs) examines
		Random rgen = new Random();

		int N = vertices.length;
		for (int i = 0; i < N; i++) {
			int z = rgen.nextInt(N);
			int temp = vertices[i];
			vertices[i] = vertices[z];
			vertices[z] = temp;
		}
	}

	public boolean maxVert(int[] vertices) { 
		Graph G = this.G;
		int vertex = vertices.length;
		int max = maxColor();
		assert vertex >= 1;
		int[] color = new int[vertex];
		int maxColor = 0;
		for (int x : vertices) {
			boolean[] compare = new boolean[maxColor + 1];
			for (int z : G.adj(x)) {
				compare[color[z]] = true;
			}
			int colorTwo = 1;
			while (colorTwo <= maxColor && compare[colorTwo]) {
				++colorTwo;
			}
			color[x] = colorTwo;
			if (colorTwo > maxColor) {
				maxColor = colorTwo;
			}
			if (maxColor >= max) {
				return false;
			}
		}
		this.color = color;
		return true;
	}

	/*
	 * private boolean maxVert(int[] vertices) { // TODO find the max number
	 * vertex Graph G = this.G; int N = vertices.length; int max = maxColor();
	 * int maxColor = 0; int[] color = new int[N]; for (int i = 0; i < N; i++) {
	 * boolean[] compare = new boolean[maxColor + 1]; for (int j : G.adj(i)){
	 * compare[color[j]] = true; } int color2 = 1; while (compare[color2] &&
	 * color2 <= maxColor){ ++color2; } color[i] = color2; if (color2 >
	 * maxColor){ maxColor = color2; } if (maxColor > max){ return false; } }
	 * this.color = color; return true; }
	 */

	// Print a warning message to System.err (not System.out).
	static void warn(String msg) {
		System.err.println("WARNING: " + msg);
	}

	// Method main() defines our command-line usage:
	//
	// java GraphColoring graph.txt [TIMELIMIT]
	//
	// The Graph filename argument must be given.
	// If omitted, the time limit defaults to 3.0 seconds.
	//
	// This program reads a Graph (in Sedgewick format) from a file,
	// constructs a GraphColoring for that Graph, and then prints that
	// initial coloring (using our toString method). Then it goes
	// into a loop with tryImprove, printing each improved coloring
	// found, until tryImprove gives up (by returning false), or until
	// we run out of time.
	public static void main(String[] args) {
		// Usage message, if not at least one argument.
		if (args.length == 0) {
			System.err
					.println("Usage: java GraphColoring graph.txt [TIMELIMIT]");
			System.exit(1);
		}
		// Read the Graph from the file named by args[0].
		Graph G = new Graph(new In(args[0]));
		// Get our time limit (in seconds).
		double secs = 3.0; // default
		if (args.length > 1)
			secs = Double.parseDouble(args[1]);

		// We are ready to start coloring, start timer.
		long start = System.currentTimeMillis();

		// Compute fast initial coloring in its constructor.
		GraphColoring coloring = new GraphColoring(G);
		// Print it.
		StdOut.println(coloring);
		if (coloring.bugs() > 0)
			warn("initial coloring has bugs!");

		// Now loop: each time that method tryImprove succeeds in
		// improving the coloring, we print it again. We stop when we
		// run out of time, or when tryImprove returns false.
		int lastK = coloring.maxColor();
		while (true) {
			// How much time have we used already? (in seconds)
			double used = (System.currentTimeMillis() - start) / 1000.0;
			if (used >= secs) // out of time?
				break;
			// Ok, try to improve the coloring.
			if (!coloring.tryImprove(secs - used))
				break; // tryImprove gave up, stop early
			// tryImprove succeeded! Print the result.
			StdOut.println(coloring);
			if (coloring.bugs() > 0)
				warn("tryImprove coloring has bugs");
			// Check that it really was an improvement.
			int K = coloring.maxColor();
			if (K >= lastK)
				warn("tryImprove returned true, but not really improved");
			lastK = K;
		}
	}
}