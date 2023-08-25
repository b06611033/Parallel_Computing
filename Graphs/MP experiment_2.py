import matplotlib.pyplot as plt
# time = [0.135022, 0.127303, 0.109672, 0.111938,
#         0.131564, 0.292516, 1.313450, 7.610269, 52.750421]
# x = [2, 3, 4, 5, 6, 7, 8, 9, 10]

time = [0.135022, 0.127303, 0.109672, 0.111938,
        0.131564, 0.292516]
x = [2, 3, 4, 5, 6, 7]

fig, ax = plt.subplots()

ax.plot(x, time)
ax.set_title("recursion level vs time")
ax.set(xlabel="recursion level", ylabel="time")

plt.legend()
plt.show()
