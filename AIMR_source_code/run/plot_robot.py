import csv
import matplotlib.pyplot as plt
import numpy as np

def read_robot_csv():
    return np.genfromtxt('robot_values.csv', delimiter=',')

def plot_values():
    plt.axis([-300, 300, -300, 300])

    while True:
        robot_coordinates = np.array(read_robot_csv())
        x, y = robot_coordinates.T
        plt.scatter(x, y, marker='o', color='#333333')
        plt.pause(0.33)

    plt.show()

if __name__ == "__main__":
    plot_values()