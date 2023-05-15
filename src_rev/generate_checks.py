# this file generates all the lines of code required
# to calculate and check the vector - matrix product

flag_len = 20
equations = 20

for i in range(equations):
    # reset accumulator
    print(f"add 800 900 900")
    
    # perform calculation
    for j in range(flag_len):
        print(f"mul 801 {str(600+j)} {str(1000+j*flag_len+i)}")
        print("add 800 800 801")
    
    # check if result is correct
    print(f"sub 800 800 {5+i}")
    print("njmp 9 800\npopout 520 20\nwrite 10\nrstout\nend\n")
