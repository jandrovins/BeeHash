with open("10beesSet.csv","r") as inf:
    with open("enxyz10.csv","w") as outf:
        lines = inf.readlines()
        for line in lines[1:]:
            xyz = line.strip("\n").split(",")
            xyz = [float(i) for i in xyz]
            outf.write("{},{},{}\n".format((xyz[0]*111111),(xyz[1]*111111),xyz[2]))
