import matplotlib.pyplot as plt
x = [1, 2, 4, 16, 64, 256, 1024]
time12 = [0.0011, 0.0016, 0.0023, 0.0065, 0.0075, 0.0284, 0.0940]
time20 = [0.1741, 0.0910, 0.0537, 0.0206, 0.0262, 0.0388, 0.1125]
time28 = [63.1122, 31.9922, 16.2932, 4.2308, 1.8053, 1.7106, 2.2618]
fig, ax = plt.subplots()
speedup12 = [0.0011/time12[i] for i in range(7)]
speedup20 = [0.1741/time20[i] for i in range(7)]
speedup28 = [63.1122/time28[i] for i in range(7)]

ax.plot(x, speedup12, label="k = 12")
ax.plot(x, speedup20, label="k = 20")
ax.plot(x, speedup28, label="k = 28")
ax.set_xscale('log', base=2)
ax.set_title("threads vs speedup")
ax.set(xlabel="threads", ylabel="speedup")

plt.legend()
plt.show()

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
