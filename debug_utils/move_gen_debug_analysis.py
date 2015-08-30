# file to compare kestrel and the BlueFever output files to find the
# differences


def read_lines_from_file(f):
    with open(f, encoding='utf-8') as a_file:
        my_list = []
        for line in a_file:
            if "**** NODE INCR - mv = " in line:
                my_list.append(line)
    return my_list


def count_occurrances(str, a_list):
    count = 0
    for line in a_list:
        if str in line:
            count += 1
    return count

def construct_dict_of_moves(my_list):
    retval = dict()
    for line in my_list:
        if "**** NODE INCR - mv = " in line:
            if line in retval:
                retval[line] += 1
            else:
                retval[line] = 1
    return retval



k_file = read_lines_from_file("/home/eddie/kkk.kkk")
print ("number of line in kkk.kkk = ", (str(len(k_file))))

v_file = read_lines_from_file("/home/eddie/vvv.vvv")
print ("number of line in kkk.kkk = ", (str(len(v_file))))

k_dict = construct_dict_of_moves(k_file)
print ("dict size of kkk.kkk = ", (str(len(k_dict))))

v_dict = construct_dict_of_moves(v_file)
print ("dict size of vvv.vvv = ", (str(len(v_dict))))

for key in v_dict.keys():
    if key not in k_dict.keys():
        print("------------------------")
        print("Not in KKK....")
        print(key)

for key in k_dict.keys():
    if key not in v_dict.keys():
        print("------------------------")
        print("Not in VVV....")
        print(key)



#    count_v = v_dict[key]
#    count_k = 0;
#    if key in k_dict:
#        count_k = k_dict[key]
#    if count_v < count_k:
#        print("------------------------")
#        print("Extra " + str(count_k - count_v) + " in KKK....total in KKK = " + str(count_k))
#        print(key)
#    if count_v > count_k:
#        print("------------------------")
#        print("Extra " + str(count_v - count_k) + " in VVV....total in VVV = " + str(count_v))
#        print(key)




