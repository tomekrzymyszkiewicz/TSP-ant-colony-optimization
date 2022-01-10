#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
import csv
import re
# 0graph_name,
# 1calculated_path,
# 2calculated_path_weight,
# 3defined_path,
# 4defined_path_weight,
# 5time,
# 6number_of_repeats,
# 7alpha,
# 8beta,
# 9rho,
# 10iterations,
# 11number_of_ants,
# 12init_tau_param,
# 13quantity_of_pheromone,
# 14evaporation_method


def main():
    if len(sys.argv) > 1:
        data = pd.read_csv(str(sys.argv[1]))
        file_name = re.sub(r"\.\\", '', str(sys.argv[1]))
    else:
        data = pd.read_csv('wyniki.csv')
        file_name = 'wyniki.csv'
    data = np.array(data)
    unique_records = []  # graph_name,number_of_repeats,alpha,beta,rho,iterations,number_of_ants,init_tau_param,quantity_of_pheromone,evaporation_method
    for record in data:
        val = [record[0], record[6], record[7], record[8], record[9],
               record[10], record[11], record[12], record[13], record[14]]
        if val not in unique_records:
            unique_records.append(val)
    print("Number of unique records: "+str(len(unique_records)))
    output = []
    for unique_record in unique_records:
        single_line = []
        single_line.append(unique_record[0])  # graph_name
        single_line.append(None)  # calculated_path
        single_line.append(None)  # calculated_path_weight
        single_line.append(None)  # defined_path
        single_line.append(None)  # defined_path_weight
        single_line.append(None)  # time
        single_line.append(unique_record[1])  # number_of_repeats
        single_line.append(unique_record[2])  # alpha
        single_line.append(unique_record[3])  # beta
        single_line.append(unique_record[4])  # rho
        single_line.append(unique_record[5])  # iterations
        single_line.append(unique_record[6])  # number_of_ants
        single_line.append(unique_record[7])  # init_tau_param
        single_line.append(unique_record[8])  # quantity_of_pheromone
        single_line.append(unique_record[9])  # evaporation_method
        calculated_path = 0
        calculated_path_weight = 0
        defined_path = ""
        defined_path_weight = 0
        time = 0.0
        for record in data:
            val = [record[0], record[6], record[7], record[8], record[9],
                   record[10], record[11], record[12], record[13], record[14]]
            if val == unique_record:
                defined_path = record[3]
                defined_path_weight = record[4]
                calculated_path = record[1]
                calculated_path_weight += record[2]
                time += record[5]
        time /= unique_record[1]
        calculated_path_weight /= unique_record[1]
        single_line[1] = calculated_path
        single_line[2] = calculated_path_weight
        single_line[3] = defined_path
        single_line[4] = defined_path_weight
        single_line[5] = time
        output.append(single_line)
    fields = ['graph_name', 'calculated_path', 'calculated_path_weight', 'defined_path', 'defined_path_weight', 'time', 'number_of_repeats',
              'alpha', 'beta', 'rho', 'iterations', 'number_of_ants', 'init_tau_param', 'quantity_of_pheromone', 'evaporation_method']
    with open('average_of_'+file_name, 'w') as file:
        write = csv.writer(file)
        write.writerow(fields)
        write.writerows(output)


if __name__ == "__main__":
    main()
