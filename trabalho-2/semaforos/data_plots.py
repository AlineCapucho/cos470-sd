import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set_style("darkgrid", {"grid.color": ".6", "grid.linestyle": ":"})

df = pd.read_csv("Runtimes.txt", sep=" ")
print(df.head(5))

parameters = list(df.groupby(['N', 'Np', 'Nc']).groups)

# Alternative ideia: https://math.stackexchange.com/a/1617683
# This post providers a infective function f : Q^2 -> R, which could be used as the variable
# in the x axis of the runtime plot

data = {
    'N': [],
    'Np-Nc': [],
    'Runtime': []
}

for parameter in parameters:
    N, Np, Nc = parameter
    parameter_values = df[(df['N'] == N) & (df['Np'] == Np) & (df['Nc'] == Nc)]['Runtime'].values
    data['N'].append(N)
    data['Np-Nc'].append(f"{Np}-{Nc}")
    data['Runtime'].append(sum(parameter_values) / len(parameter_values))

df2 = pd.DataFrame(data)
print(df2.head(5))

sns.pointplot(data=df2, x="Np-Nc", y="Runtime", hue="N")
plt.title("Runtime X Np-Nc")
plt.xlabel("Np-Nc")
plt.ylabel("Runtime (s)")
plt.savefig("Runtime X Np-Nc.png")
plt.show()

