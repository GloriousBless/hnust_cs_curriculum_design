import pandas as pd
import numpy as np
import random

from matplotlib import pyplot as plt

# 读取 Excel 文件
data = pd.read_excel('kmeans.xlsx',header=None)  # 读取数据

# 提取 x 和 y 值
x_values = data.iloc[:, 0].values
y_values = data.iloc[:, 1].values

# 合并 x 和 y 值为数据点
points = np.array(list(zip(x_values, y_values)))


# 定义一个函数来计算数据点到聚类中心的距离
def distance(point1, point2):
    return np.sqrt(np.sum((point1 - point2) ** 2))


# 定义 K-means 算法
def kmeans(points, k, max_iterations=100):
    # 从数据点中随机选择 k 个点作为初始聚类中心
    centroids = points[random.sample(range(len(points)), k)]

    for _ in range(max_iterations):
        # 分配每个数据点到最近的聚类中心
        clusters = {i: [] for i in range(k)}
        for point in points:
            distances = [distance(point, centroid) for centroid in centroids]
            cluster_index = np.argmin(distances)
            clusters[cluster_index].append(point)

        # 更新聚类中心为每个簇的平均值
        new_centroids = []
        for cluster_index, cluster_points in clusters.items():
            new_centroid = np.mean(cluster_points, axis=0)
            new_centroids.append(new_centroid)

        # 如果聚类中心几乎不再改变，则停止迭代
        if np.allclose(centroids, new_centroids):  # 比较两个数组是否非常接近
            break

        centroids = np.array(new_centroids)

    return centroids, clusters


# 设定聚类数 k
k = 3

# 执行 K-means 算法
final_centroids, final_clusters = kmeans(points, k)

# 打印最终的聚类中心和簇
# 打印每个簇的数据点
print(f"  通过k-means算法得到最终的{k}个簇，分别为：")
for i, cluster_points in final_clusters.items():
    print(f"  Cluster {i+1}：")
    print(f"    簇中心：{final_centroids[i]}")
    print("    Points：", end='')
    for point in cluster_points:
        print(point, end=",")
    print()

# 可视化数据点和聚类中心
plt.figure(figsize=(8, 6))

# 绘制数据点
for i, cluster_points in final_clusters.items():
    cluster_points = np.array(cluster_points)
    plt.scatter(cluster_points[:, 0], cluster_points[:, 1], label=f'Cluster {i+1}', s=70)

# 绘制聚类中心
plt.scatter(final_centroids[:, 0], final_centroids[:, 1], color='black', marker='x', s=180, label='Centroids')

plt.title('K-means Clustering')
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.legend()
plt.grid(False)
plt.show()
