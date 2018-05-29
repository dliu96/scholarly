import neurolab as nl
import numpy as np
from random import shuffle
import tensorflow as tf

class DeepRLAgent(object):
    def __init__(self, observation_space_dim, action_space,
                 learning_rate=0.1,
                 discount=0.99,
                 exploration_rate=0.5,
                 exploration_decay_rate=0.99,
                 batch_size=10):
        # Create train samples
        self.input_size=observation_space_dim
        self.output_size=action_space.n
        self._batch_size=batch_size

        #define and initialize your network here
        #UNCOMMENT THESE LINES TO TEST TENSORFLOW
        self._sess = tf.Session()
        self._discount = tf.constant(discount)
        self._sess.run([tf.initialize_all_variables()])

        self._saver = tf.train.Saver()

        self._n_actions = action_space.n
        self._exploration_rate = exploration_rate
        self._exploration_decay = exploration_decay_rate

    def save(self, filename):
        #raise NotImplementedError('***Error: save to file  not implemented')
        #YOUR CODE HERE: save trained model to file
        self._saver.save(self._sess, filename)
        print("Model saved in path: %s" ,filename)


    def load(self, filename):
        #raise NotImplementedError('***Error: load from file not implemented')
        #YOUR CODE HERE: load trained model from file
        self._saver.restore(self._sess, filename)
        print("Model restored from path: %s" ,filename)

    def reset(self):
        # YOUR CODE HERE: load trained model from file
        self._exploration_rate *= self._exploration_decay

    def act(self, observation):
        #raise NotImplementedError('***Error: load from file not implemented')
        # YOUR CODE HERE: pick actual best action
        if np.random.random_sample() < self._exploration_rate:
            return np.random.randint(0, self._n_actions)
        else:
            res = self._sess.run([self._policy], feed_dict={self._state: np.array([observation])})
            return res[0][0]

    def update(self, observation, action, new_observation, reward):
        # YOUR CODE HERE: pick actual best action
        self._sess.run([self._update], feed_dict={
            self._state: np.array([observation]),
            self._action: np.array([action]),
            self._next_state: np.array([new_observation]),
            self._reward: np.array([reward])
        })
