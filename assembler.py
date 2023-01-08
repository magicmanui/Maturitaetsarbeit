from cProfile import label
from multiprocessing.sharedctypes import Value
import os

fname = input("input filename:\n")
fpath = os.path.join(os.getcwd(), fname)
programm = []
instructionSet = {
    "lda": 0,
    "ldb": 8,
    "ldc": 16,
    "ldio": 24,
    "sta": 32,
    "stb": 40,
    "stcla": 48,
    "stcha": 49,
    "stclb": 50,
    "stchb": 51,
    "stio": 56,
    "add": 64,
    "sub": 72,
    "and": 80,
    "or": 88,
    "nor": 96,
    "inv": 104,
    "jmp": 112,
    "jz": 120,
    "jnz": 121,
    "jc": 122,
    "jnc": 123
}
regs = {
    "a": 0,
    "b": 1,
    "io": 2,
    "c": 5
}

with open(fpath) as f:
    origin = 0
    ai = []
    labels = []
    labelAddr = []
    content = f.read()
    lines = content.replace(',', ' ').split('\n')
    for line in lines:
        instr = line.split()
        print(instr)
        i = [0]
        if(len(instr) == 0):
            pass
        if(len(instr) == 1):
            try:
                i[0] = int(instr[0], base = 0)
            except:
                if(instr[0] == "stcla" or instr[0] == "stcha" or instr[0] == "stclb" or instr[0] == "stchb"):
                    i[0] = instructionSet[instr[0]]
                elif(instr[0] != "nop"):
                    labels.append(instr[0][0:len(instr[0])-1])
                    labelAddr.append(len(ai))
                    i.pop(0)
        elif(len(instr) == 2):
            if(instr[0] == "org"):
                origin = int(instr[1], base=0)
                i.pop(0)
            else:
                i[0] = instructionSet[instr[0]]
                x = regs.get(instr[1])
                if x is not None:
                    i[0] += x
                else:
                    try:
                        i.append(int(instr[1], base = 0))
                        i[0] += 7
                    except:
                        if i[0] < 105:
                            i[0] += 3
                        i.append(instr[1][0:len(instr[1])] + "1")
                        i.append(instr[1][0:len(instr[1])] + "2")
        elif(len(instr) == 3):
            i[0] = instructionSet[instr[0]]
            if(instr[1] == "b"):
                i[0] += 4
                i.append(int(instr[2], base = 0))
            else:
                i[0] += 3
                i.append(int(instr[1], base = 0))
                i.append(int(instr[2], base = 0))
        elif(len(instr) == 4):
            i[0] = instructionSet[instr[0]]
            x = regs.get(instr[1])
            i.append(int(instr[2], base = 0))
            i.append(int(instr[3], base = 0))
            if x is not None:
                i[0] += x
            else:
                i[0] += 7
                i.append(int(instr[1], base = 0))


            
        for j in i:
            ai.append(j)
    for i in range(len(labelAddr)):
        labelAddr[i] += origin
    for i in range(len(ai)):
        if type(ai[i]) == str:
            for j in range(len(labels)):
                if ai[i] == labels[j]+"1":
                    ai[i] = labelAddr[j] % 256
                elif ai[i] == labels[j]+"2":
                    ai[i] = labelAddr[j] // 256
with open(fname[0:len(fname)-4], 'w') as f:
    for i in ai:
        x = hex(i)[2:len(hex(i))]
        if(len(x) == 1):
            x = "0" + x
        f.write(x + " ")
            

print(ai)