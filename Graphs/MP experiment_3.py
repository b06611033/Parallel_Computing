import matplotlib.pyplot as plt
time = [62, 116, 98, 92, 74, 69, 52, 52, 56, 53, 49,
        51, 51, 56, 59, 52, 59, 55, 53, 56, 58, 54, 51, 53]
x = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
     14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24]

fig, ax = plt.subplots()

# ax.plot(x, time)
# ax.set_title("threads vs time")
# ax.set(xlabel="threads", ylabel="time")

# plt.legend()
# plt.show()

speedup = [62/time[i] for i in range(24)]


# ax.plot(x, speedup)
# ax.set_title("speedup")
# ax.set(xlabel="threads", ylabel="speedup")

# plt.legend()
# plt.show()

eff = [speedup[i]/x[i] for i in range(24)]

ax.plot(x, eff)
ax.set_title("efficiency")
ax.set(xlabel="threads", ylabel="efficiency")


plt.legend()

plt.show()
