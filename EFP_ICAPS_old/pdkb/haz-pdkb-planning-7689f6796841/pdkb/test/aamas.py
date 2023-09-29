
import sys, random, time

from krrt.utils import write_file, append_file, load_CSV

from pdkb.kd45 import *
from pdkb.pinf import *
from pdkb.rml import *
from pdkb.test.utils import random_pdkb, random_rml

TYPE = 'small'
NUM_PDKBS = 100
QUERIES_PER_PDKB = 100

if 'small' == TYPE:
    AGENTS = (2,3)
    DEPTH = (2,3)
    FLUENTS = map(Literal, 'pqr')
    FLUENT_RANGE = (2,3)
    RMLS = (3,8)

elif 'normal' == TYPE:
    AGENTS = (3,5)
    DEPTH = (4,7)
    FLUENTS = map(Literal, 'pqrst')
    FLUENT_RANGE = (3,5)
    RMLS = (13,39)

elif 'big' == TYPE:
    AGENTS = (3,10)
    DEPTH = (3,10)
    FLUENTS = map(Literal, 'pqrstvwxyz')
    FLUENT_RANGE = (5,10)
    RMLS = (50,150)

else:
    assert False, "Bad experiment type: %s" % TYPE

def now():
    return time.time()


def doit():

    skip_ag = 0
    skip_dep = 0

    if skip_ag == 0 and skip_dep == 0:
        write_file('aamas.csv', 'agents,depth,fluents,inf-size,closed-size,reduced-size,inf-query,closed-query,reduced-query')

    for ag in range(AGENTS[0], AGENTS[1]+1):
        for dep in range(DEPTH[0], DEPTH[1]+1):

            if ag < skip_ag:
                continue
            elif ag == skip_ag and dep < skip_dep:
                continue

            print
            print "--------------"
            print "   %d x %d" % (ag, dep)
            print "--------------"
            (times, sizes) = get_size_and_time(ag, dep, FLUENTS)
            print
            print "-------------------------"

            append_file('aamas.csv', "\n%d,%d,%d,%f,%f,%f,%f,%f,%f" % (ag, dep, len(FLUENTS), sizes[0], sizes[1], sizes[2], times[0], times[1], times[2]))

    #csv = ['agents,depth,fluents,reduced-rmls,closed-rmls,inf-size,closed-size,reduced-size,inf-pre,closed-pre,inf-query,closed-query,reduced-query,result']
    #csv_yes = [csv[0]]
    #csv_no = [csv[0]]

    #kbs.append(random_pdkb(random.randint(DEPTH[0], DEPTH[1]),
    #                               random.randint(AGENTS[0], AGENTS[1]),
    #                               FLUENTS[:random.randint(FLUENT_RANGE[0], FLUENT_RANGE[1])],
    #                               random.randint(RMLS[0], RMLS[1]),
    #                               False))

    #write_file('aamas.csv', csv)
    #write_file('aamas-no.csv', csv_no)
    #write_file('aamas-yes.csv', csv_yes)
    #write_file('aamas-all.csv', csv_yes + csv_no[1:])


def get_size_and_time(agents, depth, fluents):

    rmls = agents * depth * 2

    print
    print "Generating %d PDKBs..." % NUM_PDKBS
    kbs = []
    progress = 10
    trial = 1
    for i in range(NUM_PDKBS):
        if trial > progress:
            print "%d%%" % progress
            progress += 10
        trial += 1
        kbs.append(random_pdkb(depth, agents, fluents, rmls, False))
        kbs[-1].prep_unclosed_query()
    print

    print "Closing PDKBs..."
    closed_kbs = [kb.copy() for kb in kbs]
    closure_time = []
    progress = 10
    trial = 1
    for kb in closed_kbs:
        if trial > progress:
            print "%d%%" % progress
            progress += 10
        trial += 1
        start = now()
        kb.close()
        assert kb.is_consistent()
        closure_time.append(now() - start)
    print

    print "Computing INFs..."
    infs = []
    inf_time = []
    for kb in kbs:
        start = now()
        infs.append(INF.PDKB2INF(kb))
        inf_time.append(now() - start)

    def run_queries(index, rml):
        start = now()
        ans1 = infs[index].query(rml)
        inf_query = now() - start
        start = now()
        ans2 = rml in closed_kbs[index].rmls
        closed_query = now() - start
        start = now()
        ans3 = kbs[index].unclosed_query(rml)
        unclosed_query = now() - start

        assert ans1 == ans2
        assert ans2 == ans3

        return (ans1, ans2, ans3, inf_query, closed_query, unclosed_query)

    print "Performing random misc queries..."
    for i in range(len(kbs)):
        for j in range(QUERIES_PER_PDKB):

            rml = random_rml(kbs[i].depth, kbs[i].agents, kbs[i].props)

            (ans1, ans2, ans3, inf_query, closed_query, unclosed_query) = run_queries(i, rml)

            #csv.append("%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%s" %
            #           (len(kbs[i].agents), kbs[i].depth, len(kbs[i].props), len(kbs[i].rmls), len(closed_kbs[i].rmls), infs[i].size(), closed_kbs[i].size(), kbs[i].size(),
            #            inf_time[i], closure_time[i], inf_query, closed_query, unclosed_query, str(ans1)))

    print "Performing random successful queries..."
    times = [0.0,0.0,0.0]
    for i in range(len(kbs)):
        for j in range(QUERIES_PER_PDKB):

            # Get a random RML from the PDKB
            rml = random.choice(list(kbs[i].rmls))
            # Get the closed set
            entailed = list(kd45_closure(rml))
            # Pick a random element
            rml = random.choice(entailed)

            (ans1, ans2, ans3, inf_query, closed_query, unclosed_query) = run_queries(i, rml)

            assert ans1 == ans2
            assert ans2 == ans3

            times[0] += inf_query
            times[1] += closed_query
            times[2] += unclosed_query

            #csv_yes.append("%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%s" %
            #               (len(kbs[i].agents), kbs[i].depth, len(kbs[i].props), len(kbs[i].rmls), len(closed_kbs[i].rmls), infs[i].size(), closed_kbs[i].size(), kbs[i].size(),
            #                inf_time[i], closure_time[i], inf_query, closed_query, unclosed_query, str(ans1)))

    print "Performing random unsuccessful queries..."
    for i in range(len(kbs)):
        for j in range(QUERIES_PER_PDKB):

            going = True
            while going:
                rml = random_rml(kbs[i].depth, kbs[i].agents, kbs[i].props)
                if rml not in kbs[i].rmls:
                    going = False

            (ans1, ans2, ans3, inf_query, closed_query, unclosed_query) = run_queries(i, rml)

            assert ans1 == ans2
            assert ans2 == ans3

            times[0] += inf_query
            times[1] += closed_query
            times[2] += unclosed_query

            #csv_no.append("%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%s" %
            #              (len(kbs[i].agents), kbs[i].depth, len(kbs[i].props), len(kbs[i].rmls), len(closed_kbs[i].rmls), infs[i].size(), closed_kbs[i].size(), kbs[i].size(),
            #               inf_time[i], closure_time[i], inf_query, closed_query, unclosed_query, str(ans1)))


    times[0] /= float(NUM_PDKBS * QUERIES_PER_PDKB * 2)
    times[1] /= float(NUM_PDKBS * QUERIES_PER_PDKB * 2)
    times[2] /= float(NUM_PDKBS * QUERIES_PER_PDKB * 2)

    sizes = []
    sizes.append(float(sum([inf.size() for inf in infs])) / float(NUM_PDKBS))
    sizes.append(float(sum([kb.size() for kb in closed_kbs])) / float(NUM_PDKBS))
    sizes.append(float(sum([kb.size() for kb in kbs])) / float(NUM_PDKBS))

    print "\nDone!\n"

    return (times, sizes)


def checkit():

    data = load_CSV('aamas.csv')[1:]

    for row in data:
        for i in range(len(row)):
            if row[i] == '0.000000':
                row[i] = '0.000001'

    def plot_data(data, inds, labs, cols, zlabel, fname):

        data_map = {}
        for ag in range(AGENTS[0], AGENTS[1]+1):
            data_map[ag] = {}
            for dep in range(DEPTH[0], DEPTH[1]+1):
                data_map[ag][dep] = {}

        for row in data:
            data_map[int(row[0])][int(row[1])][inds[0]] = float(row[inds[0]])
            data_map[int(row[0])][int(row[1])][inds[1]] = float(row[inds[1]])
            data_map[int(row[0])][int(row[1])][inds[2]] = float(row[inds[2]])

        from mpl_toolkits.mplot3d import axes3d
        import matplotlib.pyplot as plt
        import matplotlib
        import numpy as np

        X, Y = np.meshgrid(np.arange(AGENTS[0], AGENTS[1]+1), np.arange(DEPTH[0], DEPTH[1]+1))

        #zs0 = np.array([1 for x,y in zip(np.ravel(X), np.ravel(Y))])
        #zs1 = np.array([data_map[x][y][ind1] / data_map[x][y][indnorm] for x,y in zip(np.ravel(X), np.ravel(Y))])
        #zs2 = np.array([data_map[x][y][ind2] / data_map[x][y][indnorm] for x,y in zip(np.ravel(X), np.ravel(Y))])

        zs0 = np.array([data_map[x][y][inds[0]] for x,y in zip(np.ravel(X), np.ravel(Y))])
        zs1 = np.array([data_map[x][y][inds[1]] for x,y in zip(np.ravel(X), np.ravel(Y))])
        zs2 = np.array([data_map[x][y][inds[2]] for x,y in zip(np.ravel(X), np.ravel(Y))])

        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')

        if 'Query Time ($log_e(sec)$)' == zlabel:
            Z0 = np.log(zs0).reshape(X.shape)
            Z1 = np.log(zs1).reshape(X.shape)
            Z2 = np.log(zs2).reshape(X.shape)
        else:
            #ax.set_zticks([])
            Z0 = (zs0 / 1000).reshape(X.shape)
            Z1 = (zs1 / 1000).reshape(X.shape)
            Z2 = (zs2 / 1000).reshape(X.shape)

        #ax.plot_wireframe(X, Y, Z0, color='0.75')
        ax.plot_wireframe(X, Y, Z0, color=cols[0])
        ax.plot_wireframe(X, Y, Z1, color=cols[1])
        ax.plot_wireframe(X, Y, Z2, color=cols[2])

        #cset = ax.contourf(X, Y, Z0, zdir='z', offset=-100, cmap=matplotlib.cm.coolwarm)
        #cset = ax.contourf(X, Y, Z0, zdir='x', offset=0, cmap=matplotlib.cm.coolwarm)
        #cset = ax.contourf(X, Y, Z0, zdir='z', offset=0, cmap=matplotlib.cm.coolwarm)
        #cset = ax.contourf(X, Y, Z0, zdir='y', offset=40, cmap=cm.coolwarm)

        ax.set_xlabel('# of Agents')
        ax.set_ylabel('Maximum Depth')
        ax.set_zlabel(zlabel)

        scatter1_proxy = matplotlib.lines.Line2D([0],[0], linestyle="none", c=cols[0], marker = 's')
        scatter2_proxy = matplotlib.lines.Line2D([0],[0], linestyle="none", c=cols[1], marker = 's')
        scatter3_proxy = matplotlib.lines.Line2D([0],[0], linestyle="none", c=cols[2], marker = 's')
        ax.legend([scatter1_proxy, scatter2_proxy, scatter3_proxy], [labs[0], labs[1], labs[2]], numpoints = 1)

        ax.get_xaxis().set_major_locator(matplotlib.ticker.MaxNLocator(integer=True))
        ax.get_yaxis().set_major_locator(matplotlib.ticker.MaxNLocator(integer=True))

        plt.show()


    col1 = '#1b9e77'
    col2 = '#d95f02'
    col3 = '#7570b3'

    print "Plotting time..."
    plot_data(data, [6, 8, 7], ['INF', '$V_{RML}$', 'Closure'], [col1, col3, col2], 'Query Time ($log_e(sec)$)', 'time.eps')

    print "Plotting size..."
    plot_data(data, [4, 3, 5], ['Closure', 'INF', '$V_{RML}$'], [col2, col1, col3], 'Size (x1000)', 'size.eps')


