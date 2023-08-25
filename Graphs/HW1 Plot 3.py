import matplotlib.pyplot as plt
x = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
y = [2.2239, 5.3655, 5.3871, 3.1569, 2.6765, 2.6839, 2.6896, 2.6890,
     2.9617, 2.7083, 2.6850, 2.6962, 2.6869, 2.7240, 2.6864, 2.6813]
fig, ax = plt.subplots()
ax.plot(x, y)
ax.set_title("ntasks-per-node vs time")
ax.set(xlabel="ntasks-per-node", ylabel="time")
plt.show()
