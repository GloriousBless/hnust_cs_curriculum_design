import pandas as pd
import numpy as np
from matplotlib import pyplot as plt

# 读取Excel文件
file_path = 'data_dbscan.xlsx'
data = pd.read_excel(file_path, header=None)

# 获取数据点，假设x和y值分别位于第一列和第二列，且从第一行开始
X = data.iloc[:, :2].values

# DBSCAN 参数设置
eps = 2  # 邻域半径
min_samples = 3  # 最小样本数


# 计算点之间的曼哈顿距离
def manhattan_distance(a, b):
    return np.sum(np.abs(a - b))


# 找到邻居点
def find_neighbors(X, point_index, eps):
    neighbors = []
    for i, point in enumerate(X):
        if manhattan_distance(X[point_index], point) <= eps:
            neighbors.append(i)
    return neighbors


# DBSCAN 算法
def dbscan(X, eps, min_samples):
    clusters = [-1] * len(X)  # 初始化簇标签为-1（噪声点）
    cluster_id = 0

    for i, point in enumerate(X):
        if clusters[i] != -1:  # 已经被标记过，跳过
            continue

        neighbors = find_neighbors(X, i, eps)

        if len(neighbors) < min_samples:
            clusters[i] = -1  # 标记为噪声点
        else:
            cluster_id += 1
            clusters[i] = cluster_id

            j = 0
            while j < len(neighbors):
                neighbor_point = neighbors[j]
                if clusters[neighbor_point] == -1:  # 未被标记过
                    clusters[neighbor_point] = cluster_id

                    new_neighbors = find_neighbors(X, neighbor_point, eps)
                    if len(new_neighbors) >= min_samples:
                        neighbors.extend(new_neighbors)

                j += 1

    return clusters


# 运行 DBSCAN 算法
result_clusters = dbscan(X, eps, min_samples)

# 根据聚类结果创建字典存储每个簇的数据点
clusters_dict = {}
for i, cluster_id in enumerate(result_clusters):
    if cluster_id not in clusters_dict:
        clusters_dict[cluster_id] = []
    clusters_dict[cluster_id].append(X[i])

# 输出簇
unique_clusters = set(result_clusters) - {-1}  # 去除噪声点簇（-1）
Point_num = 0
print(f"通过DBSCAN算法聚类得到{len(unique_clusters)}个簇，如下：")
for cluster_id in unique_clusters:
    cluster_points = clusters_dict[cluster_id]
    print(f"Cluster {cluster_id}:")
    for item in cluster_points:
        print(item, end=',')
    print('')
    Point_num += len(cluster_points)
print(f"点的数目为：{Point_num}")

# 输出噪声点
j = 0
flag = 0
for i in result_clusters:
    if i == -1:
        if flag == 0:
            print("噪声点：", end='')
            flag = 1
        print(X[j], end=',')
        j += 1
if j == 0:
    print("No noise points found.")

