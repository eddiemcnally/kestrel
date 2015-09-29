# file to compare kestrel and the BlueFever output files to find the
# differences


def construct_dict_of_moves_with_details(f):
    with open(f, encoding='utf-8') as a_file:
        counter = 0
        inmovelist = False
        currentline = ''
        my_dict = dict()
        for line in a_file:

            if "QQQ moves generated for depth " in line:
                if counter is not 0:
                    #print(str(counter))
                    if currentline in list(my_dict.keys()):
                        print(("dup key in " + f + " : " + currentline))
                    my_dict[currentline] = counter
                #print(line)
                currentline = line
                inmovelist = False
                counter = 0
            if "MoveList:" in line:
                inmovelist = True
            if inmovelist:
                counter = counter + 1
        #print(str(len(my_dict.keys())))
    return my_dict


def construct_set_of_moves(f):
    with open(f, encoding='utf-8') as a_file:
        my_set = set()
        for line in a_file:
            if "QQQ moves generated for depth " in line:
                if line in my_set:
                    print(("dup key in " + f + " : " + line))
                else:
                    my_set.add(line)
    return my_set


k_file = construct_set_of_moves("/home/eddie/kkk.kkk")
print("# keys in kkk.kkk : " + str(len(k_file)))

v_file = construct_set_of_moves("/home/eddie/vvv.vvv")
print("# keys in vvv.vvv : " + str(len(v_file)))

for v in v_file:
    if v not in k_file:
        print("not in k: " + v)

for k in k_file:
    if v not in k_file:
        print("not in v: " + k)


#    if v_file[v] != k_file[v]:
#        print("line: " + v)
#        print("k = " + str(k_file[v]) + " v = " + str(v_file[v]))
#        print("\n\n\n\n\n")





