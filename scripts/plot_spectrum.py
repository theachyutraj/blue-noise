import csv
import sys
from pathlib import Path
import matplotlib.pyplot as plt

experiment_path = "experiments" / Path(sys.argv[1])

radii = []
powers = []

with open(experiment_path / "radial_power.csv", newline="") as file:
    reader = csv.reader(file)

    for row in reader:
        radii.append(int(row[0]))
        powers.append(float(row[1]))

plt.plot(radii, powers)
plt.xlabel("Spatial frequency radius (bins)")
plt.ylabel("Mean power")
plt.title("Radial Power Spectrum - " + sys.argv[1])
plt.grid(True)
plt.tight_layout()
plt.savefig(experiment_path / "radial_power.png", dpi=150)
plt.show()