import matplotlib.pyplot as plt
from typing import List, Dict

def plotGraph(Plots:Dict[int, float], graphName:str):
    bufferSize:List[int] = []
    timeElapse:List[float] = []
    
    for k,v in Plots.items():
        bufferSize.append(k)
        timeElapse.append(v)
    
    plt.plot(bufferSize, timeElapse)
    plt.scatter(bufferSize, timeElapse)
    plt.xlabel('Buffer Size')
    plt.ylabel('Time Elapse')
    
    plt.title(graphName)
    plt.xscale('log')
    plt.show()
    
    plt.savefig(f'/home/xyma/MarxFiles/Project1/copy/benchmark_timespec/{graphName}.png')
    plt.close()
    
if __name__ == "__main__":
    Plots:Dict[int, float]= {}
    #buffersize -> time
    while(True):
        line = input()
        if line == "exit":
            break
        numbers = line.split(':')
        print(numbers)
        if(len(numbers) <2 ):
            continue
        Plots[int(numbers[0])] = float(numbers[1])
    
    
    graphName:str = input("input the graph name: ")
    plotGraph(Plots, graphName)
    