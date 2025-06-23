import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv("trajectory.csv", header=None,
                 names=["raw_x", "raw_y", "kf_x", "kf_y", "true_x", "true_y"])

rmse_x = np.sqrt(np.mean((df["kf_x"] - df["true_x"])**2))
rmse_y = np.sqrt(np.mean((df["kf_y"] - df["true_y"])**2))
rmse_total = np.sqrt(rmse_x**2 + rmse_y**2)

plt.figure(figsize=(10, 6))
plt.scatter(df["raw_x"], df["raw_y"], label="GPS Measurement", color="red", alpha=0.4, s=20)
plt.plot(df["kf_x"], df["kf_y"], label="Kalman Filter Estimate", color="green", linewidth=2)
plt.plot(df["true_x"], df["true_y"], label="True Path", color="black", linestyle="--", linewidth=2)

rmse_text = f"RMSE (X): {rmse_x:.2f}\nRMSE (Y): {rmse_y:.2f}\nTotal RMSE: {rmse_total:.2f}"
plt.text(0.05, 0.95, rmse_text, transform=plt.gca().transAxes,
         fontsize=10, verticalalignment='top', bbox=dict(boxstyle="round,pad=0.3", edgecolor='gray', facecolor='white', alpha=0.7))

plt.title("Projectile Tracking with Kalman Filter")
plt.xlabel("X position")
plt.ylabel("Y position")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
