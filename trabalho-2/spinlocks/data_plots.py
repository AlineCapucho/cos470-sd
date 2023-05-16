import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set_style("darkgrid", {"grid.color": ".6", "grid.linestyle": ":"})

df = pd.read_csv("Runtimes.txt", sep=" ")
print(df.head(5))

parameters = list(df.groupby(['N', 'K']).groups)

# Alternative ideia: https://math.stackexchange.com/a/1617683
# This post providers a infective function f : Q^2 -> R, which could be used as the variable
# in the x axis of the runtime plot

data = {
    'N': [],
    'K': [],
    'Runtime': []
}

for parameter in parameters:
    N, K = parameter
    parameter_values = df[(df['N'] == N) & (df['K'] == K)]['Runtime'].values
    data['N'].append(N)
    data['K'].append(f"{K}")
    data['Runtime'].append(sum(parameter_values) / len(parameter_values))

df2 = pd.DataFrame(data)
print(df2.head(5))

sns.pointplot(data=df2, x="K", y="Runtime", hue="N")
plt.title("Runtime X K")
plt.xlabel("K")
plt.ylabel("Runtime (s)")
plt.savefig("Runtime X K.png")
plt.show()

