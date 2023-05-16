import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set_style("darkgrid", {"grid.color": ".6", "grid.linestyle": ":"})

series_list = []

with open("buffer_usage.txt", "r") as f:
    lines = f.readlines()[1:]
    lines = [line[2:-2].split(" ") for line in lines]
    lines = [list(map(int, line)) for line in lines]
    series_length = [len(line) for line in lines]
    min_series_length = min(series_length)
    for i in range(0, len(lines), 10):
        series = np.array(lines[i][:min_series_length])
        for j in range(1, 10):
            try:
                series = series + np.array(lines[i+j][:min_series_length])
            except:
                print(np.array(lines[i+j]))
                print(series.shape, np.array(lines[i+j]).shape)
                print(i, j)
                sys.exit()
        series = series / 10
        series_list.append(series)

df = pd.read_csv("Runtimes.txt", sep=" ")

parameters = list(df.groupby(['N', 'Np', 'Nc']).groups)

data_series = []

data = {
    'N': [],
    'Np-Nc': [],
    'Time_Series': []
}

for i in range(len(parameters)):
    N, Np, Nc = parameters[i]
    data['N'].append(N)
    data['Np-Nc'].append(f"{Np}-{Nc}")
    data['Time_Series'].append(series_list[i])

for i in range(0, len(parameters), 7):
    for j in range(0, 7):
        plt.plot([k for k in range(len(data['Time_Series'][0]))], data['Time_Series'][i+j], label=f"{data['Np-Nc'][i+j]}")
    plt.title(f"Buffer Occupation X Index, N = {data['N'][i]}")
    plt.xlabel("Index")
    plt.ylabel("Buffer Occupation")
    # plt.legend()
    plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
    plt.savefig(f"Buffer Occupation, N={data['N'][i]}.png", bbox_inches="tight")
    plt.close("all")