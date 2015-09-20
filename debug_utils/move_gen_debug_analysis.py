# file to compare kestrel and the BlueFever output files to find the
# differences


def construct_dict_of_moves(f):
    with open(f, encoding='utf-8') as a_file:
        counter = 0
        inmovelist = False
        currentline = ''
        my_dict = dict()
        for line in a_file:

            if "+++++++++++++ moves generated for depth " in line:
                if counter is not 0:
                    #print(str(counter))
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


k_file = construct_dict_of_moves("/home/eddie/kkk.kkk")
v_file = construct_dict_of_moves("/home/eddie/vvv.vvv")

print(str(len(k_file.keys())))
print(str(len(v_file.keys())))

for v in v_file.keys():
    if v not in k_file.keys():
        print("not in k: " + v)
    if v_file[v] != k_file[v]:
        print("k = " + str(k_file[v]) + " v = " + str(v_file[v]))





