import numpy as np

def main():
    sampl = np.random.uniform(low=-49.0, high=49.0, size=(100,))
    sampl2 = np.random.uniform(low=-49.0, high=49.0, size=(100,))
    for i in range(1, len(sampl)):
        print("treeLocations.push_back(make_pair(%ff, %ff));" % 
                (sampl[i], sampl2[i]))

main()
