import matplotlib.pyplot as plt

algorithms = []
values = []

# Read results
with open("results.txt", "r") as file:
    for line in file:
        name, value = line.split()
        algorithms.append(name)
        values.append(float(value))

# Create bar chart
plt.bar(algorithms, values)

plt.title("Average Waiting Time Comparison")
plt.xlabel("Scheduling Algorithms")
plt.ylabel("Average Waiting Time")

# Save image
plt.savefig("chart.png")

# Show chart (optional)
plt.show()