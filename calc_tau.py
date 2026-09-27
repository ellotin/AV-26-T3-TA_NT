import pandas as pd
import numpy as np

df = pd.read_csv('step_test.csv', header=None, skiprows=1)
time = df[0].values.astype(float)
u = df[1].values.astype(float)
y = df[2].values.astype(float)

start_idx = np.where(u > 0)[0][0]
moving_idx = np.where(y > y[start_idx] + 0.05)[0][0]
t_motion_start = time[moving_idx]

steady_y = np.mean(y[-int(len(y)*0.1):])
initial_y = y[moving_idx]
total_change = steady_y - initial_y
target_value = initial_y + 0.632 * total_change

target_idx = moving_idx + np.argmin(np.abs(y[moving_idx:] - target_value))
t_63 = time[target_idx]
tau = t_63 - t_motion_start

print("--- AUTOMATIC TAU RESULT ---")
print(f"Motion start time: {t_motion_start} s")
print(f"63.2% target timestamp: {t_63} s")
print(f"Your True Tau (tau): {tau:.4f} seconds")
