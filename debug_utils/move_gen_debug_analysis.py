# file to compare kestrel and the BlueFever output files to find the
# differences


def read_lines_from_file(f):
    with open(f, encoding='utf-8') as a_file:
        my_list = []
        for line in a_file:
            if "**** NODE INCR" in line:
                my_list.append(line)
    return my_list

def construct_set_of_moves(my_list):
    retval = set()
    for line in my_list:
        if "**** NODE INCR" in line:
            retval.add(line)
    return retval



k_file = read_lines_from_file("/home/eddie/kkk.kkk")
print ("number of lines in kkk.kkk = ", (str(len(k_file))))

v_file = read_lines_from_file("/home/eddie/vvv.vvv")
print ("number of lines in kkk.kkk = ", (str(len(v_file))))

k_set = construct_set_of_moves(k_file)
print ("set size of kkk.kkk = ", (str(len(k_set))))

v_set = construct_set_of_moves(v_file)
print ("set size of vvv.vvv = ", (str(len(v_set))))

for line in v_set:
    if line not in k_set:
        print("------------------------")
        print("Not in KKK....")
        print(line)

for line in k_set:
    if line not in v_set:
        print("------------------------")
        print("Not in VVV....")
        print(line)



