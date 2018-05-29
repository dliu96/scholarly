# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for 
# educational purposes provided that (1) you do not distribute or publish 
# solutions, (2) you retain this notice, and (3) you provide clear 
# attribution to UC Berkeley, including a link to 
# http://inst.eecs.berkeley.edu/~cs188/pacman/pacman.html
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero 
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and 
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called
by Pacman agents (in searchAgents.py).
"""

import util

class Node:

    """A node in a search tree. Contains a pointer to the parent (the node
    that this is a successor of) and to the actual state for this node. Note
    that if a state is arrived at by two paths, then there are two nodes with
    the same state.  Also includes the action that got us to this state, and
    the total path_cost (also known as g) to reach the node.  Other functions
    may add an f and h value; see best_first_graph_search and astar_search for
    an explanation of how the f and h values are handled. You will not need to
    subclass this class."""

    def __init__(self, state, parent=None, action=None, path_cost=0, corners=0):
        """Create a search tree Node, derived from a parent by an action."""
        self.state = state
        self.parent = parent
        self.action = action
        self.path_cost = path_cost
        self.corners = corners
        self.depth = 0
        if parent:
            self.depth = parent.depth + 1

    def __repr__(self):
        return "<Node {}>".format(self.state)

    def __lt__(self, node):
        return self.state < node.state

    def solution(self):
        """Return the sequence of actions to go from the root to this node."""
        return [node.action for node in self.path()[1:]]

    def path(self):
        """Return a list of nodes forming the path from the root to this node."""
        node, path_back = self, []
        while node:
            path_back.append(node)
            node = node.parent
        return list(reversed(path_back))

    def getCost(self):
        '''print self.path_cost'''
        return self.path_cost

    # We want for a queue of nodes in breadth_first_search or
    # astar_search to have no duplicated states, so we treat nodes
    # with the same state as equal. [Problem: this may not be what you
    # want in other contexts.]

    def __eq__(self, other):
        return isinstance(other, Node) and self.state == other.state

    def __hash__(self):
        return hash(self.state)


class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples,
        (successor, action, stepCost), where 'successor' is a
        successor to the current state, 'action' is the action
        required to get there, and 'stepCost' is the incremental
        cost of expanding to that successor
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.  The sequence must
        be composed of legal moves
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other
    maze, the sequence of moves will be incorrect, so only use this for tinyMaze
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return [s, s, w, s, w, w, s, w]


def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first

    Your search algorithm needs to return a list of actions that reaches
    the goal.  Make sure to implement a graph search algorithm

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:
    """

    "*** YOUR CODE HERE ***"

    return tree_search(problem, util.Stack())


def breadthFirstSearch(problem):
    """
    Search the shallowest nodes in the search tree first.
    """
    "*** YOUR CODE HERE ***"
    return tree_search(problem, util.Queue())

"Given a problem and an abstract datatype, this method returns a list of moves leading to a goal state."
def tree_search(problem, fringe):
    "Keep track of visited nodes so that we don't backtrack"
    visted_nodes = []

    "Put the initial state of the problem onto the fringe"
    fringe.push(Node(problem.getStartState()))

    "While the fringe is not empty"
    while fringe:
        "Get the next node off the fringe"
        node = fringe.pop()

        "Check to see if this node has already been visted... If so, skip"
        if node in visted_nodes:
            continue

        "If we haven't come across this node before, add it to the visited list now"
        visted_nodes.append(node)

        '''print node.path()'''

        "We found a goal state!"
        if problem.isGoalState(node.state):
            solution = node.path()
            moves = []
            "We must return a valid list of directions to get to this goal state"
            for direction in solution:
                if direction.action is not None:
                    moves.append(direction.action)
            print len(solution)
            return moves

        "We have not found a goal state... Go through each possible direction and add them to the fringe"
        for child in problem.getSuccessors(node.state):
            child_node = Node(child[0],node,child[1])
            if child_node not in visted_nodes:
                fringe.push(child_node)

    return None

"Very similar to tree search... Just keeping track of costs this time"
def cost_search(problem,fringe,heuristic=None):
    visted_nodes = []
    start_state = Node(problem.getStartState())
    fringe.push(start_state, start_state.getCost())

    while fringe:
        node = fringe.pop()

        if node in visted_nodes:
            continue

        visted_nodes.append(node)

        solution = node.path()
        moves = []

        for direction in solution:
            if direction.action is not None:
                moves.append(direction.action)

        pcost = node.getCost()


        if problem.isGoalState(node.state):
            return moves

        for child in problem.getSuccessors(node.state):

            child_node = Node(child[0], node, child[1], pcost + child[2])

            '''print "Child: ", child'''
            if child_node not in visted_nodes:

                fcost = child_node.getCost()

                '''print child_node, "pcost = ", pcost, "fcost = ", fcost'''
                if heuristic:
                    fringe.push(child_node, fcost + heuristic(child_node.state,problem))
                else:
                    fringe.push(child_node, fcost)



    return None


def uniformCostSearch(problem):
    "Search the node of least total cost first. "
    "*** YOUR CODE HERE ***"

    node = Node(problem.getStartState())
    return cost_search(problem, util.PriorityQueue())

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0


def aStarSearch(problem, heuristic=nullHeuristic):
    "Search the node that has the lowest combined cost and heuristic first."
    "*** YOUR CODE HERE ***"
    node = Node(problem.getStartState())
    return cost_search(problem, util.PriorityQueue(),heuristic)


# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
