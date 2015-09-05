# utility to compare output of roce chess engine and kestrel
# for a run of the "divide perft" test



def read_lines_from_ROCE_file(f):
    "Read in lines and return a dict"
    with open(f, encoding='utf-8') as a_file:
        my_dict = dict()
        for line in a_file:
            s = line.strip()
            if s:
                #print(line)
                val = line.split()
                #print(val)
                my_dict[val[0].strip()] = val[1].strip()
    return my_dict


def read_lines_from_KESTREL_file(f):
    "Read in lines and return a dict"
    with open(f, encoding='utf-8') as a_file:
        my_dict = dict()
        for line in a_file:
            s = line.strip()
            if s:
                #print(line)
                val = line.split(":")
                #print(val)
                my_dict[val[0].strip()] = val[1].strip()
    return my_dict


k_file = read_lines_from_KESTREL_file("/home/eddie/kkk.kkk")
print("# Kestrel moves : " + str(len(k_file.keys())))
#for k in k_file.keys():
#    print("KKK key : " + k)

r_file = read_lines_from_ROCE_file("/home/eddie/rrr.rrr")
print("# ROCE moves : " + str(len(r_file.keys())))
#for k in r_file.keys():
#    print("RRR key : " + k)


for key in k_file.keys():
    if key in r_file.keys():
        if (k_file[key] != r_file[key]):
            print("Diff : move = " + key)
            print("\tKestrel # = " + str(k_file[key]))
            print("\tROCE # = " + str(r_file[key]))
    else:
        print("Move " + key + " not in ROCE")


for key in r_file.keys():
    if key in k_file.keys():
        if (r_file[key] != r_file[key]):
            print("Diff : move = " + key)
            print("\tKestrel # = " + str(k_file[key]))
            print("\tROCE # = " + str(r_file[key]))
    else:
        print("Move " + key + " not in KESTREL")



