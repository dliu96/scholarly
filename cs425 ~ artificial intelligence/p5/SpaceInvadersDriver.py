import gym
import numpy as np

# hyperparameters
EPISODES = 2000
WINDOW = 200
ConvergeThreshold=0.0001

# model initialization
D = 80 * 80 # input dimensionality: 80x80 grid

def prepro(I):
  """ prepro 210x160x3 uint8 frame into 6400 (80x80) 1D float vector """
  I = I[35:195] # crop
  I = I[::2,::2,0] # downsample by factor of 2
  I[I == 144] = 0 # erase background (background type 1)
  I[I == 109] = 0 # erase background (background type 2)
  I[I != 0] = 1 # everything else (paddles, ball) just set to 1
  return I.astype(np.float).ravel()

def sigmoid(x):
  return 1.0 / (1.0 + np.exp(-x)) # sigmoid "squashing" function to interval [0,1]

env = gym.make('SpaceInvaders-v0')
for i_episode in range(5):
    observation = env.reset()
    while True:
        t = 0
        env.render()  # comment out for faster training!
        cur_x = prepro(observation)
        # print(cur_x)

        # print(observation)
        action = env.action_space.sample()
        observation, reward, done, info = env.step(action)
        if done:
            print("Episode finished after {} timesteps".format(t + 1))
            break
env.close()



