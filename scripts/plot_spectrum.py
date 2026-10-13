import csv
import matplotlib.pyplot as plt

radii = []
powers = []

with open("radial_power.csv", newline="") as file:
    reader = csv.reader(file)

    for row in reader:
        radii.append(int(row[0]))
        powers.append(float(row[1]))

plt.plot(radii, powers)
plt.xlabel("Spatial frequency radius (bins)")
plt.ylabel("Mean power")
plt.title("Radial Power Spectrum — White Noise")
plt.grid(True)
plt.tight_layout()
plt.savefig("radial_power.png", dpi=150)
plt.show()