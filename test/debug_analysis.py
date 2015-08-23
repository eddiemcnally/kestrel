

def read_lines_from_file(f):
    infile = open(f, "r")
    #print(infile)
    my_list = []
    for line in infile.readlines():
        my_list.append(line)
    return my_list


def count_occurrances(str, a_list):
    count = 0
    for line in a_list:
        if str in line:
            count += 1
    return count


k_file = read_lines_from_file("/home/eddie/kkk.kkk")
print ("number of line in kkk.kkk = ", (str(len(k_file))))




v_file = read_lines_from_file("/home/eddie/vvv.vvv")
print ("number of line in kkk.kkk = ", (str(len(v_file))))


