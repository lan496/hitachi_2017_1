import argparse
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt


def G_and_Gemb(V, E, u, v, w, Vemb, Eemb, a, b):
    # Graph G with 'weight' attribute
    G = nx.Graph()
    G.add_nodes_from(range(V))
    for uu, vv, ww in zip(u, v, w):
        G.add_edge(uu, vv, weight=ww)

    # Graph Gemb with 'pos' attribute
    Gemb = nx.Graph()
    length = int(np.sqrt(Vemb))
    for i in range(Vemb):
        Gemb.add_node(i, pos=(i // length, i % length))
    for aa, bb in zip(a, b):
        Gemb.add_edge(aa, bb)

    return G, Gemb


def draw_before(G, Gemb):
    G_edge_labels = dict([((uu, vv), d['weight']) for uu, vv, d
                          in G.edges(data=True)])
    G_pos = nx.circular_layout(G)

    Gemb_pos = dict([(i, d['pos']) for i, d in Gemb.nodes(data=True)])

    f, (ax1, ax2) = plt.subplots(1, 2)

    nx.draw_networkx_edge_labels(G, G_pos,
                                 edge_labels=G_edge_labels, ax=ax1)
    nx.draw_networkx_labels(G, G_pos, ax=ax1)
    nx.draw(G, G_pos, ax=ax1)

    nx.draw(Gemb, ax=ax2, pos=Gemb_pos)

    plt.show()


def draw_after(G, Gemb, s, t):
    for ss, tt in zip(s, t):
        Gemb.node[tt]['embed'] = ss

    for uu, vv in G.edges(data=False):
        aa = t[np.where(s == uu)][0]
        bb = t[np.where(s == vv)][0]

        if Gemb.has_edge(aa, bb):
            Gemb.edge[aa][bb]['embed'] = G.edge[uu][vv]['weight']
            G.edge[uu][vv]['embed'] = G.edge[uu][vv]['weight']

    # labels for G
    G_edge_labels = dict([((uu, vv), d['weight']) for uu, vv, d in G.edges(data=True)])
    G_used_edge_labels = dict([((uu, vv), d['embed']) for uu, vv, d in G.edges(data=True) if 'embed' in d])
    G_used_edgelist = [(uu, vv) for uu, vv, d in G.edges(data=True) if 'embed' in d]
    G_pos = nx.circular_layout(G)

    # labels for Gemb
    Gemb_used_labels = dict([(i, d['embed']) for i, d in Gemb.nodes(data=True) if 'embed' in d])

    Gemb_used_edge_labels = dict([((aa, bb), d['embed']) for aa, bb, d in Gemb.edges(data=True) if 'embed' in d])
    Gemb_used_edgelist = [(aa, bb) for aa, bb, d in Gemb.edges(data=True) if 'embed' in d]
    Gemb_pos = dict([(i, d['pos']) for i, d in Gemb.nodes(data=True)])

    f, (ax1, ax2) = plt.subplots(1, 2)

    # draw G
    nx.draw_networkx_edge_labels(G, pos=G_pos, edge_labels=G_edge_labels, ax=ax1)
    nx.draw_networkx_edge_labels(G, pos=G_pos, edge_labels=G_used_edge_labels, ax=ax1, font_color='r')
    # nx.draw_networkx_edges(G, pos=G_pos, edgelist=G_used_edgelist, edge_color='b')
    nx.draw_networkx_labels(G, G_pos, ax=ax1)
    nx.draw(G, G_pos, ax=ax1, node_color='g')

    # draw Gemb
    nx.draw(Gemb, ax=ax2, pos=Gemb_pos, node_color='g')
    nx.draw_networkx_labels(Gemb, ax=ax2, labels=Gemb_used_labels, pos=Gemb_pos)
    nx.draw_networkx_edge_labels(Gemb, pos=Gemb_pos, edge_labels=Gemb_used_edge_labels, ax=ax2, font_color='r')
    # nx.draw_networkx_edges(Gemb, pos=Gemb_pos, edgelist=Gemb_used_edgelist, ax=ax2, edge_color='b')

    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Visualize Given Graphs.')
    parser.add_argument('--input', required=True)
    parser.add_argument('--output', default=None)

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
                         for i in range(Eemb)]).T
        a -= 1
        b -= 1

    if args.output:
        # read output
        with open(args.output, "r") as f:
            s, t = np.array([[int(x) for x in next(f).split()]
                             for i in range(V)]).T
            s -= 1
            t -= 1

    G, Gemb = G_and_Gemb(V, E, u, v, w, Vemb, Eemb, a, b)

    if not args.output:
        draw_before(G, Gemb)
    else:
        draw_after(G, Gemb, s, t)
