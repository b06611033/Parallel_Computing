import matplotlib.pyplot as plt
x = [1, 2, 4, 8, 16, 32, 64]
timeparallel = [2.640637, 2.708961, 2.806929,
                2.895175, 3.022656, 3.124896, 3.358026]
time = [2.636681, 5.421776, 11.172654,
        23.018146, 47.227964, 96.623328, 199.002970]
fig, ax = plt.subplots()

# ax.plot(x, timeparallel)
# ax.set_xscale('log', base=2)
# ax.set_title("time")
# ax.set(xlabel="process", ylabel="speedup")

# plt.legend()
# plt.show()

speedup = [time[i]/timeparallel[i] for i in range(7)]


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
