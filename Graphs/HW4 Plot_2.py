import matplotlib.pyplot as plt
timeparallel = [2.638959, 1.316662, 0.666031,
                0.342886, 0.185023, 0.112663, 0.068802]
x = [1, 2, 4, 8, 16, 32, 64]

fig, ax = plt.subplots()

# ax.plot(x, timeparallel)
# ax.set_xscale('log', base=2)
# ax.set_title("time")
# ax.set(xlabel="process", ylabel="speedup")

# plt.legend()
# plt.show()

speedup = [2.638959/timeparallel[i] for i in range(7)]


# ax.plot(x, speedup)
# ax.set_xscale('log', base=2)
# ax.set_title("speedup")
# ax.set(xlabel="process", ylabel="speedup")

# plt.legend()
# plt.show()

eff = [speedup[i]/x[i] for i in range(7)]

ax.plot(x, eff)
ax.set_xscale('log', base=2)
ax.set_title("efficiency")
ax.set(xlabel="process", ylabel="efficiency")


plt.legend()

plt.show()
