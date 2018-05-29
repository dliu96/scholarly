##################################################################
#################################--------------------------------
##
##
##     THIS CODE IS WRITTEN BY:
##
##                      DEREK LIU
##                          &
##                      ALLEN SHIU
##
##
#################################--------------------------------
##################################################################

import gym
import sys
import csv
import DeepRLAgent
import ast

# calculate the maximum Q-Value for each action
def maxQ(state):
    stay=0
    hit=0
    if (state, '0') in qVals:
        stay = qVals[(state, '0')]
    if (state, '1') in qVals:
        hit = qVals[(state, '1')]
    return max(stay, hit)

# calculate the best action based on our Q-Value
def maxA(state):
    stay = 0
    hit = 0
    if (state, '0') in qVals:
        stay = qVals[(state, '0')]
    if (state, '1') in qVals:
        hit = qVals[(state, '1')]

    # choose action based on highest q-val
    if stay<hit:
        return 1
    else:
        return 0



env = gym.make('Blackjack-v0')
#See details about the game rules here, but not necessary for your agent -- it will learn the rules by experimentation!
#Environment definition: https://github.com/openai/gym/blob/master/gym/envs/toy_text/blackjack.py
#actions, observations described in detail above
#so your policy network needs to learn to predict one of these actions based on the observation.


rlAgent = DeepRLAgent.DeepRLAgent
qVals = {}

#train
if 'train' in sys.argv[1]:
    #print("train!")
    csvfile = open("test.csv", "w+")

    with csvfile:
        fieldnames = ["episode", "action", "observation", "reward", "done", "total_rewards"]
        writer = csv.DictWriter(csvfile, fieldnames = fieldnames)
        writer.writeheader()

        for i_episode in range(10000000):

            total_rewards = 0
            observation = env.reset()

            #create a row of data to add to our file
            row = {}
            row['episode'] = i_episode

            while True:
                t=0
                #env.render()  #comment out for faster training!

                action = env.action_space.sample() # random action, use your own action policy here
                observation, reward, done, info = env.step(action)

                row['action'] = action
                row['observation'] = observation
                row['reward'] = reward
                row['done'] = done

                # write each row
                writer.writerow(row)

                total_rewards+=reward
                t+=1
                if done:
                    #print("Episode finished after {} timesteps %d with reward %d ", t, total_rewards)
                    row['total_rewards'] = total_rewards
                    writer.writerow(row)
                    break



    # exit our csv
    csvfile.close()


#test
else:
    # open the same file
    with open("test.csv", "r") as csvfile:

        reader = csv.DictReader(csvfile)
        for row in reader:
            #print(row['episode'], row['action'], row['observation'], row['reward'], row['done'], row['total_rewards'])

            state = ast.literal_eval(row['observation'])
            action = row['action']
            reward = float(row['reward'])


            # check if our q-values actually exist
            if (state, action) not in qVals:
                qVals[(state,action)] = 0.0 + 0.1 *((reward) + 0.99*maxQ(state) - 0)
            else:
                qVals[(state, action)] = qVals[(state, action)] + 0.1 * ((reward) + 0.99 * maxQ(state) - qVals[(state, action)])
            #print(str(qVals))

        wins = 0

        # play 10 times
        for i_episode in range(10):

            total_rewards = 0
            observation = env.reset()
            while True:
                t=0
                #env.render()  #comment out for faster training!

                action = maxA(observation) #random action, use your own action policy here

                observation, reward, done, info = env.step(action)

                total_rewards+=reward
                t+=1
                if (total_rewards == 1): wins+=1
                if done:
                    print("Episode finished after {} timesteps %d with reward %d ", t, total_rewards)
                    #print("Win rate:", (wins/(i_episode+1)))
                    break



env.close()


