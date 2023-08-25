import matplotlib.pyplot as plt
x = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192]
y1 = [1.2846, 0.6463, 0.3307, 0.1653, 0.0858, 0.0461, 0.0420,
      0.0360, 0.0368, 0.0371, 0.0416, 0.0685, 0.1342, 0.2753]
fig, ax = plt.subplots(2, 2)
ax[0][0].plot(x, y1)
ax[0][0].set_xscale('log', base=2)
ax[0][0].set_title("1.1 threads vs time")
ax[0][0].set(ylabel="time")

y2 = [1.2846/y1[i] for i in range(14)]
ax[0][1].plot(x, y2)
ax[0][1].set_xscale('log', base=2)
ax[0][1].set_title("1.2 threads vs speedup")
ax[0][1].set(xlabel="threads", ylabel="speedup")

y3 = [y2[i]/x[i] for i in range(14)]
ax[1][0].plot(x, y3)
ax[1][0].set_xscale('log', base=2)
ax[1][0].set_title("1.3 threads vs efficiency")
ax[1][0].set(xlabel="threads", ylabel="efficiency")

plt.show()

x = [1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000]
y = [0.0476, 0.0101, 0.00429, 0.00121, 0.00129, 0.000208, 0.00000804]
fig, ax = plt.subplots()
ax.plot(x, y)
ax.set_xscale('log', base=10)
ax.set(xlabel="n", ylabel="error")

plt.show()
