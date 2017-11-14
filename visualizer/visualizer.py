import argparse
import networkx as nx


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Visualize Given Graphs.')
    parser.add_argument('input')
    parser.add_argument('output')

    args = parser.parse_args()

    # read input
    with open(args.input, "r") as f:
        V, E = [int(x) for x in next(f).split()]
        u = []
        v = []
        w = []
        for i in range(E):
            uu, vv, ww = [int(x) for x in next(f).split()]
            uu -= 1
            vv -= 1
            u.append(uu)
            v.append(vv)
            w.append(ww)
        Vemb, Eemb = [int(x) for x in next(f).split()]
        a = []
        b = []
        for i in range(E):
            aa, bb = [int(x) for x in next(f).split()]
            aa -= 1
            bb -= 1
            a.append(aa)
            b.append(bb)

    # read output
    with open(args.output, "r") as f:
        s = []
        t = []
        for i in range(V):
            ss, tt = [int(x) for x in next(f).split()]
            ss -= 1
            tt -= 1
            s.append(ss)
            t.append(tt)
