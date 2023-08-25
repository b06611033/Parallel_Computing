import matplotlib.pyplot as plt
x = [2, 3, 4, 5, 6, 7, 8, 9, 10]
time = [0.006947, 0.006630, 0.006653, 0.012339,
        0.029749, 0.143372, 1.117617, 6.804479, 57.902258]
fig, ax = plt.subplots()

ax.plot(x, time)
ax.set_title("k vs time")
ax.set(xlabel="k", ylabel="time")

plt.legend()
plt.show()
