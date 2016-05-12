import os
import random

cur = "pp2au"

pp = list()
au = list()
def init():
    fpp = open("id.txt","r")
    fau = open("auid.txt","r")
    while True:
        line = fpp.readline()
        if not line:
            break
        pp.append(int(line))
    while True:
        line = fau.readline()
        if not line:
            break
        au.append(int(line))
    fpp.close()
    fau.close()
    
def check():
    os.system("./" + cur + ".out")
    os.system("./" + cur + "_test.out")
    fans = open("ans.json", "r")
    fout = open("out.json", "r")
    
    ans = eval(fans.readline())
    out = eval(fout.readline())
    
    fans.close()
    fout.close()
    ans.sort()
    out.sort()

    flag = True
    for a in ans:
        if a not in out:
            print "ans:",a
            flag = False
    for b in out:
        if b not in ans:
            print "out:",b
            flag = False
    return flag

init()
os.system("make " + cur);
os.system("make " + cur + "_test")
while True:
    fin = open("in.json", "w")
    fin.write(str(random.choice(pp)) + " " +  str(random.choice(au)))
    fin.close()sh
    if not check():
        break
    else:
        print "OK"

