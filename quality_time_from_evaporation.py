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
                       'calculated_path_weight', 'defined_path_weight', 'time', 'evaporation_method'])
    data = np.array(data)
    x = np.array([data[i][1].count(' ') for i in range(0, len(data))])
    vertices = x.reshape((-1,1))
    data = np.concatenate([data,vertices],axis=1)
    x = list(set(x))
    x.sort()
    y_quality_QAS = [100*(data[i][2]-data[i][3])/data[i][2]
                      for i in range(len(data)) if data[i][5] == 'QAS']
    y_quality_CAS = [100*(data[i][2]-data[i][3])/data[i][2]
                        for i in range(len(data)) if data[i][5] == 'CAS']
    y_time_QAS = [data[i][4]
                   for i in range(len(data)) if data[i][5] == 'QAS']
    y_time_CAS = [data[i][4]
                     for i in range(len(data)) if data[i][5] == 'CAS']
    col1 = 'steelblue'
    col2 = 'red'
    _, ax_time = plt.subplots()
    _, ax_quality = plt.subplots()
    plt.figure(1)
    ax_quality.plot(x, y_quality_QAS, marker='o', label='QAS')
    ax_quality.plot(x, y_quality_CAS, marker='o', label='CAS')
    ax_quality.set_ylabel('Stosunek błędu do wartości optymalnej [%]')
    ax_quality.set_xlabel('Liczba wierzchołków w grafie')
    ax_quality.legend()
    plt.figure(2)
    ax_time.plot(x, y_time_QAS, marker='o', label='QAS')
    ax_time.plot(x, y_time_CAS, marker='o', label='CAS')
    ax_time.set_ylabel('Czas wykonania algorytmu [s]')
    ax_time.set_xlabel('Liczba wierzchołków w grafie')
    ax_time.legend()
    plt.show()


if __name__ == "__main__":
    main()
