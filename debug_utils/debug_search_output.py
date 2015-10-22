# utility to compare output of bluefever software search algo and that
# of kestrel, to see differences

def read_lines_from_file(f):
    "Read in lines and return a dict"
    with open(f, encoding='utf-8') as a_file:
        my_list = list()
        for line in a_file:
            if line.startswith("depth"):
                s = line.strip()
                my_list.append(s)
    return my_list

k_file = read_lines_from_file("/home/eddie/kkk.kkk")
print(("lines in k " + str(len(k_file))))

v_file = read_lines_from_file("/home/eddie/vvv.vvv")
print(("lines in v " + str(len(v_file))))

for k_line in k_file:
    if k_line not in v_file:
        print("Not in VVV = " + k_line)

for v_line in v_file:
    if v_line not in k_file:
        print("Not in KKK = " + v_line)
