import matplotlib.pyplot as plt
x = [1, 2, 4, 16, 64, 256, 1024]
time12 = [0.0292, 0.0093, 0.0060, 0.0159, 0.0357, 0.2907, 11.3232]
time20 = [0.1809, 0.1009, 0.0582, 0.0259, 0.0324, 0.2620, 11.4644]
time28 = [62.6588, 32.0785, 16.2498, 4.2256, 2.1592, 2.2895, 3.7784]
fig, ax = plt.subplots()
speedup12 = [0.0292/time12[i] for i in range(7)]
speedup20 = [0.1809/time20[i] for i in range(7)]
speedup28 = [62.6588/time28[i] for i in range(7)]

# ax.plot(x, speedup12, label="k = 12")
# ax.plot(x, speedup20, label="k = 20")
# ax.plot(x, speedup28, label="k = 28")
# ax.set_xscale('log', base=2)
# ax.set_title("threads vs speedup")
# ax.set(xlabel="threads", ylabel="speedup")

# plt.legend()
# plt.show()

eff12 = [speedup12[i]/x[i] for i in range(7)]
eff20 = [speedup20[i]/x[i] for i in range(7)]
eff28 = [speedup28[i]/x[i] for i in range(7)]
ax.set_xscale('log', base=2)
ax.set_title("threads vs efficiency")
ax.set(xlabel="threads", ylabel="efficiency")

ax.plot(x, eff12, label="k = 12")
ax.plot(x, eff20, label="k = 20")
ax.plot(x, eff28, label="k = 28")

plt.legend()

plt.show()
