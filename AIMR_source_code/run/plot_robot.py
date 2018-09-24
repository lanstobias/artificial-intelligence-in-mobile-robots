import csv
import matplotlib.pyplot as plt
import numpy as np

def read_robot_csv():
    return np.genfromtxt('robot_values.csv', delimiter=',')

def plot_values():
    plt.axis([-50, 300, -50, 300])
    #fig=plt.figure()
    #fig.suptitle('ePuck Movement', fontsize=20)

    robot_coordinates = np.array(read_robot_csv())
    x, y = robot_coordinates.T
    plt.scatter(x, y, marker='o', facecolors='none', edgecolor='#555555', s=250)
    plt.xlabel('mm', fontsize=18)
    plt.ylabel('mm', fontsize=16)
    plt.grid()
    #fig.savefig('ePuckmovementlab3.jpg')
    plt.show()
   

if __name__ == "__main__":
    plot_values()