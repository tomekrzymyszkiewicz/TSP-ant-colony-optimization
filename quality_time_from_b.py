import sys
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt


def main():
    if len(sys.argv) > 1:
        file_name = str(sys.argv[1])
    else:
        file_name = 'wyniki.csv'
    data = pd.read_csv(file_name, usecols=['graph_name', 'calculated_path',
                       'calculated_path_weight', 'defined_path_weight', 'time', 'beta'])
    data = np.array(data)
    x = np.array([data[i][1].count(' ') for i in range(0, len(data))])
    vertices = x.reshape((-1,1))
    data = np.concatenate([data,vertices],axis=1)
    x = list(set(x))
    x.sort()
    y_from_beta = {} #time,quality
    for test in data:
        if not test[5] in y_from_beta:
            y_from_beta[test[5]] = []
    for y in y_from_beta:
        y_from_beta[y] = [[100*(data[i][2]-data[i][3])/data[i][2] for i in range(len(data)) if data[i][5] == y],[data[i][4] for i in range(len(data)) if data[i][5] == y]]
    _, ax_time = plt.subplots()
    _, ax_quality = plt.subplots()
    for y in y_from_beta:
        plt.figure(1)
        ax_time.plot(x,y_from_beta[y][1],label=f'beta={str(y)}',marker='o')
        plt.figure(2)
        ax_quality.plot(x,y_from_beta[y][0],label=f'beta={str(y)}',marker='o')
    ax_time.legend()
    ax_time.set_xlabel('Liczba wierzchołków w grafie')
    ax_time.set_ylabel('Czas wykonania algorytmu [s]')
    ax_quality.legend()
    ax_quality.set_xlabel('Liczba wierzchołków w grafie')
    ax_quality.set_ylabel('Stosunek błędu do wartości optymalnej [%]')
    plt.show()


if __name__ == "__main__":
    main()
