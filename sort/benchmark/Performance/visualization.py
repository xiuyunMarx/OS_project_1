import matplotlib.pyplot as plt
import re
from typing import List, Dict

def extract_numbers(s):
    return [float(num) if '.' in num else int(num) for num in re.findall(r'-?\d+\.?\d*', s)]

def vis_multi():
    Plots:Dict[int, Dict[int,float]] = {}
        #number of threads to Dict[array length, time]
    with open("/home/xyma/MarxFiles/Project1/sort/benchmark/Performance/multi.txt","r") as f:
        for line in f:
            if line == "\n":
                continue
            numbers = extract_numbers(line)
            if numbers[1] not in Plots:
               Plots[numbers[1]] = {}
            Plots[numbers[1]][numbers[0]] = numbers[2]
    
    # Plot the data
    for threadNum, data in Plots.items():
        x = list(data.keys())
        y = list(data.values())
        plt.plot(x, y, label=f'{threadNum} Thread')
        plt.scatter(x, y)

    plt.xlabel('Array Length')
    plt.ylabel('Time')
    plt.title('Performance Visualization')
    plt.legend()
    plt.show()
    plt.savefig('/home/xyma/MarxFiles/Project1/sort/benchmark/Performance/Multi_visualization.png')
    plt.close()
    
def vis_single():
    Plots:Dict[int, float] = {}
        #Dict[array length, time]
    with open("/home/xyma/MarxFiles/Project1/sort/benchmark/Performance/single.txt","r") as f:
        for line in f:
            if line == "\n":
                continue
            numbers = extract_numbers(line)
            Plots[numbers[0]] = numbers[1]
    

    # Plot the data
    arrayLength = list(Plots.keys())
    Time = list(Plots.values())
    plt.plot(arrayLength, Time)
    plt.scatter(arrayLength,Time)

    plt.xlabel('Array Length')
    plt.ylabel('Time')
    plt.title('Performance Visualization')
    plt.legend()
    plt.show()
    plt.savefig('/home/xyma/MarxFiles/Project1/sort/benchmark/Performance/Single_visualization.png')
    plt.close()
    
    
if __name__ == "__main__":
    vis_multi()
    vis_single()