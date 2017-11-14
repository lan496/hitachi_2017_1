import argparse
# import networkx as nx
import numpy as np


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Visualize Given Graphs.')
    parser.add_argument('input')
    parser.add_argument('output')

    args = parser.parse_args()

    # read input
    with open(args.input, "r") as f:
        V, E = [int(x) for x in next(f).split()]
        u, v, w = np.array([[int(x) for x in next(f).split()]
                            for i in range(E)]).T
        u -= 1
        v -= 1

        Vemb, Eemb = [int(x) for x in next(f).split()]
        a, b = np.array([[int(x) for x in next(f).split()]
                         for i in range(E)]).T
        a -= 1
        b -= 1

    # read output
    with open(args.output, "r") as f:
        s, t = np.array([[int(x) for x in next(f).split()]
                         for i in range(V)]).T
        s -= 1
        t -= 1
