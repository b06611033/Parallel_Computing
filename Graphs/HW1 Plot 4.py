import matplotlib.pyplot as plt
x = [100, 10000, 1000000, 100000000]
y1 = [0.0001, 0.0001, 0.0037, 0.3434]
y2 = [0.0003, 0.0027, 0.0404, 0.0284]
y = [y1[i]/y2[i] for i in range(4)]
y3 = [0.00000265, 0.000000000265, 0.0000000000000262, 0.000000000000000283]
fig, ax = plt.subplots()
# ax.set_xscale('log', base=10)
# ax.set_title("n intervals vs speedup")
# ax.set(xlabel="n intervals", ylabel="speedup")
# ax.plot(x, y)

ax.set_xscale('log', base=10)
ax.set_yscale('log', base=10)
ax.set_title("n intervals vs error")
ax.set(xlabel="n intervals", ylabel="error")
ax.plot(x, y3)
plt.show()
