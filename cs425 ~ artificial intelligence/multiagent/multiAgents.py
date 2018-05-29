# multiAgents.py
# --------------
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
#
# Modified by Eugene Agichtein for CS325 Sp 2014 (eugene@mathcs.emory.edu)
#

from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        Note that the successor game state includes updates such as available food,
        e.g., would *not* include the food eaten at the successor state's pacman position
        as that food is no longer remaining.
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        currentFood = currentGameState.getFood() #food available from current state
        newFood = successorGameState.getFood() #food available from successor state (excludes food@successor) 
        currentCapsules=currentGameState.getCapsules() #power pellets/capsules available from current state
        newCapsules=successorGameState.getCapsules() #capsules available from successor (excludes capsules@successor)
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        foods = currentFood.asList()
        distance = -float("inf")

        if successorGameState.isWin():
            return float("inf")

        if action is Directions.STOP:
            return -float("inf")

        for state in newGhostStates:
            position = state.getPosition()
            if newPos == position:
                return -float("inf")

        for food in foods:
            # use manhattan distance as our heuristic
            manhattanDist = -manhattanDistance(food, newPos)

            if manhattanDist > distance:
                distance = manhattanDist

        return distance





def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        bestState = self.minimax(gameState, 0, 0)
        bestValue = bestState[0]
        return bestValue

    def minimax(self, gameState, agentIndex, depth2):
        numAgents = gameState.getNumAgents()

        # fix wrap around cases for agents
        if numAgents <= agentIndex:
            agentIndex = 0
            depth2 += 1

        if depth2 is self.depth or gameState.isWin() or gameState.isLose():
            return self.evaluationFunction(gameState)

        if agentIndex is not self.index:
            return self.minValue(gameState, agentIndex, depth2)
        else:
            return self.maxValue(gameState, agentIndex, depth2)

    def minValue(self, gameState, agentIndex, depth2):

        # declare a value variable
        value = ("", float("inf"))

        legalActions = gameState.getLegalActions(agentIndex)

        if not legalActions:
            return self.evaluationFunction(gameState)

        for action in legalActions:
            if action is not Directions.STOP:
                thisState = self.minimax(gameState.generateSuccessor(agentIndex, action), agentIndex + 1, depth2)
                if type(thisState) is tuple:
                    thisState = thisState[1]

                newValue = min(value[1], thisState)

                if newValue is not value[1]:
                    value = (action, newValue)

        return value

    def maxValue(self, gameState, agentIndex, depth2):
        value = ("", -float("inf"))

        legalActions = gameState.getLegalActions(agentIndex)

        if not legalActions:
            return self.evaluationFunction(gameState)

        for action in legalActions:
            if action is not Directions.STOP:
                thisState = self.minimax(gameState.generateSuccessor(agentIndex, action), agentIndex + 1, depth2)

                if type(thisState) is tuple:
                    thisState = thisState[1]

                newValue = max(value[1], thisState)

                if newValue is not value[1]:
                    value = (action, newValue)
        return value


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        a = -float("inf")
        b = float("inf")
        bestState = self.minimax(gameState, 0, 0, a, b)
        bestValue = bestState[0]
        return bestValue

    def minimax(self, gameState, agentIndex, depth2, a, b):
        numAgents = gameState.getNumAgents()

        if numAgents <= agentIndex:
            agentIndex = 0
            depth2 += 1

        if depth2 is self.depth or gameState.isWin() or gameState.isLose():
            return self.evaluationFunction(gameState)
        
        # ghosts
        if agentIndex is not self.index:
            return self.minValue(gameState, agentIndex, depth2, a, b)
        # pacman
        else:
            return self.maxValue(gameState, agentIndex, depth2, a, b)

    def minValue(self, gameState, agentIndex, depth2, a, b):
        value = ("", float("inf"))

        legalActions = gameState.getLegalActions(agentIndex)

        if not legalActions:
            return self.evaluationFunction(gameState)

        for action in legalActions:
            
            if action is not Directions.STOP:
                thisState = self.minimax(gameState.generateSuccessor(agentIndex, action), agentIndex + 1, depth2, a, b)
                
                if type(thisState) is tuple:
                    thisState = thisState[1]

                newValue = min(value[1], thisState)

                if newValue is not value[1]:
                    value = (action, newValue)

                if value[1] < a:
                    return value

                b = min(b,value[1])

        return value

    def maxValue(self, gameState, agentIndex, depth2, a, b):
        value = ("", -float("inf"))

        legalActions = gameState.getLegalActions(agentIndex)

        if not legalActions:
            return self.evaluationFunction(gameState)

        for action in legalActions:
            if action is not Directions.STOP:
                thisState = self.minimax(gameState.generateSuccessor(agentIndex, action), agentIndex + 1, depth2, a, b)

                if type(thisState) is tuple:
                    thisState = thisState[1]

                newValue = max(value[1], thisState)

                if newValue is not value[1]:
                    value = (action, newValue)

                if value[1] > b:
                    return value

                a = max(a,value[1])

        return value


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        bestState = self.expectimax(gameState, 0, 0)
        bestValue = bestState[0]
        return bestValue

    def expectimax(self, gameState, agentIndex, depth2):
        numAgents = gameState.getNumAgents()

        if numAgents <= agentIndex:
            agentIndex = 0
            depth2 += 1

        if depth2 is self.depth or gameState.isWin() or gameState.isLose():
            return self.evaluationFunction(gameState)

        #ghost
        if agentIndex is not self.index:
            return self.expValue(gameState, agentIndex, depth2)
        #pacman
        else:
            return self.maxValue(gameState, agentIndex, depth2)

    def expValue(self, gameState, agentIndex, depth2):
        value = ("", float("inf"))

        legalActions = gameState.getLegalActions(agentIndex)

        if not legalActions:
            return self.evaluationFunction(gameState)

        for action in legalActions:
            if action is not Directions.STOP:
                thisState = self.expectimax(gameState.generateSuccessor(agentIndex, action), agentIndex + 1, depth2)
                if type(thisState) is tuple:
                    thisState = thisState[1]

                newValue = min(value[1], thisState)

                # calculate probability
                probability = newValue/len(gameState.getLegalActions(agentIndex))
                newValue += probability * newValue

                if newValue != value[1]:
                    value = (action, newValue)

        return value

    def maxValue(self, gameState, agentIndex, depth2):
        value = ("", -float("inf"))

        legalActions = gameState.getLegalActions(agentIndex)

        if not legalActions:
            return self.evaluationFunction(gameState)

        for action in legalActions:
            if action is not Directions.STOP:
                thisState = self.expectimax(gameState.generateSuccessor(agentIndex, action), agentIndex + 1, depth2)

                if type(thisState) is tuple:
                    thisState = thisState[1]

                newValue = max(value[1], thisState)

                if newValue is not value[1]:
                    value = (action, newValue)
        return value

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>

      I wanted to prioritize ghost hunting so the weight to catch a scared ghost is meant to reflect that interest.
      Otherwise, pacman just tries to get the closest food
    """
    "*** YOUR CODE HERE ***"

    ### WEIGHTS ###
    w_FOOD = 10.0
    w_GHOST = 10.0

    # what we're really counting on to score points
    w_SCAREDGHOST = 100.0


    # Useful information you can extract from a GameState (pacman.py)
    pacmanPos = currentGameState.getPacmanPosition()
    foods = currentGameState.getFood()
    ghostStates = currentGameState.getGhostStates()

    # set up our variable before all the weights are counted in
    heuristic = scoreEvaluationFunction(currentGameState)

    # iterate through each ghost on the map and calculate the distance
    v_GHOST = 0
    for ghost in ghostStates:
        # manhattanDistance
        distance = manhattanDistance(pacmanPos, ghostStates[0].getPosition())

        # as long as we still have a chance...
        if distance > 0:
            # heavily prioritize scared ghosts
            if ghost.scaredTimer != 0:
                v_GHOST += w_SCAREDGHOST / distance
            # now we should be the scared ones...
            else:
                v_GHOST -= w_GHOST / distance
    heuristic += v_GHOST

    # find our closest food
    distancesToFood = [manhattanDistance(pacmanPos, food) for food in foods.asList()]
    if len(distancesToFood):
        # weight the closest food more
        heuristic += w_FOOD / min(distancesToFood)
    # if there is only one food left...
    else:
        # GET IT BY ANY MEANS NECESSARY
        return float("inf")

    return heuristic

# Abbreviation
better = betterEvaluationFunction

class ContestAgent(MultiAgentSearchAgent):
    """
      Your agent for the mini-contest
    """

    def getAction(self, gameState):
        """
          Returns an action.  You can use any method you want and search to any depth you want.
          Just remember that the mini-contest is timed, so you have to trade off speed and computation.

          Ghosts don't behave randomly anymore, but they aren't perfect either -- they'll usually
          just make a beeline straight towards Pacman (or away from him if they're scared!)
        """
        "*** YOUR CODE HERE ***"
        util.raiseNotDefined()
