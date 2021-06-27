import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns


df_dijkstra = pd.read_csv("src/backend/bench-dijkstra.csv", header=None)
df_astar_euclidean = pd.read_csv("src/backend/bench-astar-euclidean.csv", header=None)
df_astar_manhattan = pd.read_csv("src/backend/bench-astar-manhattan.csv", header=None)

df = pd.DataFrame({"Dijkstra": df_dijkstra[2], # type: ignore
                   "A* with Euclidean Metric Heuristic": df_astar_euclidean[2], # type: ignore
                   "A* with Manhattan Metric Heuristic": df_astar_manhattan[2]}) # type: ignore

plt.figure(figsize=(10, 4))
sns.violinplot(data=df, kind="violin", )
plt.ylabel("average duration of 30 executions in ms")
plt.savefig("report/benchmark_violinplot.png", dpi=300)
