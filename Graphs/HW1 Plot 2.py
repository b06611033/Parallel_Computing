import matplotlib.pyplot as plt
x = [1, 2, 4, 8, 16, 32, 64]
y1 = [0.3444, 0.8793, 0.4476, 0.2239, 0.1211, 0.1051, 0.0400]
fig, ax = plt.subplots(2, 2)
ax[0][0].plot(x, y1)
ax[0][0].set_xscale('log', base=2)
ax[0][0].set_title("5.1. processes vs time")
ax[0][0].set(ylabel="time")

y2 = [0.3444/y1[i] for i in range(7)]
ax[0][1].plot(x, y2)
ax[0][1].set_xscale('log', base=2)
ax[0][1].set_title("5.2. processes vs speedup")
ax[0][1].set(xlabel="processes", ylabel="speedup")

y3 = [y2[i]/x[i] for i in range(7)]
ax[1][0].plot(x, y3)
ax[1][0].set_xscale('log', base=2)
ax[1][0].set_title("5.3. processes vs efficiency")
ax[1][0].set(xlabel="processes", ylabel="efficiency")

plt.show()
