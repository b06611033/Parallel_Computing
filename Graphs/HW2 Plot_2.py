import matplotlib.pyplot as plt
x = [1, 2, 4, 16, 64, 256, 1024]
time20 = [0.1741, 0.0910, 0.0537, 0.0206, 0.0262, 0.0388, 0.1125]
time20q = [0.1736, 0.1735, 0.1722, 0.1773, 0.1756, 0.1737, 0.1724]
time28 = [63.1122, 31.9922, 16.2932, 4.2308, 1.8053, 1.7106, 2.2618]
time28q = [62.7099, 62.6570, 62.6791, 62.6779, 62.6779, 62.7339, 62.6432]
fig, ax = plt.subplots(2)

ax[0].plot(x, time20, label="parallel merge sort")
ax[0].plot(x, time20q, label="quick sort")
ax[0].set_xscale('log', base=2)
ax[0].set_title("threads vs time; k = 20")
ax[0].set(ylabel="time")

ax[1].plot(x, time28, label="parallel merge sort")
ax[1].plot(x, time28q, label="quick sort")
ax[1].set_xscale('log', base=2)
ax[1].set_title("threads vs time; k = 28")
ax[1].set(xlabel="threads", ylabel="time")

plt.legend()
plt.show()
