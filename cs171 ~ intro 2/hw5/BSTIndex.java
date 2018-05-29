/*
THIS CODE WAS MY OWN WORK , IT WAS WRITTEN WITHOUT CONSULTING
CODE WRITTEN BY OTHER STUDENTS . Derek Liu
 */

public class BSTIndex {

	private Node parent;

	public class Node {
		String index;
		MovieInfo info;
		Node left, right;

		public Node(String k, MovieInfo m) {
			this.index = k;
			this.info = m;
		}
	}

	public BSTIndex() {
		parent = null;
	}

	public MovieInfo findExact(String index) {
		Node x = parent;
		while (x != null) {
			int c = index.compareToIgnoreCase(x.index);
			if (c < 0) {
				x = x.left;
			} else if (c > 0) {
				x = x.right;
			} else {
				return x.info;
			}
		}

		return null;
	}

	public MovieInfo findPrefix(String N) {
		N = N.substring(0, N.indexOf('*'));
		Node x = parent;
		while (x != null) {
			N = N.toLowerCase();
			x.index = x.index.toLowerCase();
			boolean there = x.index.startsWith(N);
			if (!there) {
				int c = N.compareToIgnoreCase(x.index);

				if (c == 0) {
					return x.info;
				} else if (c < 0) {
					x = x.left;
				} else {
					x = x.right;
				}
			} else {
				return x.info;
			}
		}

		return null;
	}

	public void insert(MovieInfo info) {
		parent = insert(parent, info);
	}

	public Node insert(Node x, MovieInfo movie) {
		if (x == null) {
			return new Node(movie.shortName, movie);
		}
		int c = movie.shortName.compareTo(x.index);
		if (c < 0) {
			x.left = insert(x.left, movie);
		} else if (c > 0) {
			x.right = insert(x.right, movie);
		} else {
			x.info = movie;
		}
		return x;
	}
}