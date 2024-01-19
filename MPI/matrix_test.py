import random

filename = "big_matrix_generator.txt"

# Clear File
with open(filename,"w") as w:
    w.write('')
    w.close()

# Write File
with open(filename,"a") as w:
    M = 40
    N = 80
    P = 120

    # Generate Matrix A
    for i in range(0,M):
        for j in range(0,N):
            w.write(str(random.randint(-10000,100000)) + " ")
        w.write("\n")

    w.write("\n\n\n\n\n\n")
    # Generate Matrix B
    for i in range(0,N):
        for j in range(0,P):
            w.write(str(random.randint(-10000,100000)) + " ")
        w.write("\n")
    
    
    w.close()

            
    