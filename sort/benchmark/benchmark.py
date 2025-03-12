import subprocess
import random
import typing
from statistics import mean

def gen_random(size:int , min:int,max:int)->typing.List[int]:
    return [random.randint(min,max) for _ in range(size)]

def benchmark_single():
    programName = "/home/xyma/MarxFiles/Homework/Sort/benchmark/MergesortSingle"
    outputFile = "/home/xyma/MarxFiles/Homework/Sort/benchmark/Performance/single.txt"
    
    with open(outputFile,"w") as f:   
        for i in range(100,700,100):
            size = i*500
            inputList = gen_random(size, -size, size)
            inputStr = str(size)  + "\n" + " ".join(map(str,inputList)) + "\n"
            
            TimeList:typing.List[float] = []
            for j in range(20):
                result = subprocess.run([programName], input=inputStr,capture_output= True, text=True)
                TimeList.append(float(result.stderr.split(' ')[2]))
            
            ans:str = f"array length:{size}, {mean(TimeList)}\n"
            print(ans)
            f.write(ans)


def benchmark_multi():   
    programName = "/home/xyma/MarxFiles/Homework/Sort/benchmark/MergesortMulti"
    outputFile = "/home/xyma/MarxFiles/Homework/Sort/benchmark/Performance/multi.txt"
    threadNums = [2,4,8,16,32]
    with open(outputFile,"w") as f:   
        for i in range(100,700,100):
            size = i*500
            inputList = gen_random(size, -size, size)
            inputStr = str(size)  + "\n" + " ".join(map(str,inputList)) + "\n"
            for threadCnt in threadNums:
                TimeList:typing.List[float] = []
                for j in range(20):
                    result = subprocess.run([programName, str(threadCnt)], input=inputStr,capture_output= True, text=True)
                    TimeList.append(float(result.stderr.split(' ')[2]))
                    
                ans:str = f"array length:{size}, thread numbers:{threadCnt}, time elapsed: {mean(TimeList):.8f}\n"
                print(ans)
                f.write(ans)
            
if __name__ == "__main__":
    benchmark_single()      
    benchmark_multi()  