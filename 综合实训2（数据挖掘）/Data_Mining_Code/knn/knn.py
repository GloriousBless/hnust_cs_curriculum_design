import pandas as pd
import math

# 读取Excel文件，读取训练数据
data = pd.read_excel('data_train.xlsx', header=None)

# 数据预处理
# 姓名是第一列，身高是第二列，等级是第三列
names = data.iloc[:, 0].values
features = data.iloc[:, 1].values
labels = data.iloc[:, 2].values


# 定义距离函数
def euclidean_distance(x1, x2):
    return math.sqrt((x1 - x2) ** 2)


# 定义K近邻算法
def k_nearest_neighbors(training_features, training_labels, test_instance, k):
    distances = []
    for i in range(len(training_features)):
        distance = euclidean_distance(test_instance, training_features[i])
        distances.append((training_features[i], distance, training_labels[i], names[i]))
    distances.sort(key=lambda x: x[1])
    neighbors = distances[:k]
    return neighbors


# 测试数据
test_data = pd.read_excel('data_test.xlsx', header=None)
test_name_all = test_data.iloc[:, 0].values
test_height_all = test_data.iloc[:, 1].values

# 设置K值
k = 5

for test_name, test_height in zip(test_name_all, test_height_all):
    # 获取K个最近邻居
    nearest_neighbors = k_nearest_neighbors(features, labels, test_height, k)

    # 输出K个最近邻居及其等级和姓名
    print(f"  {test_name}的K个最近邻居及其等级和姓名:")
    for neighbor in nearest_neighbors:
        print(f"  姓名: {neighbor[3]}, 身高: {neighbor[0]}m, 距离: {round(neighbor[1], 3)}, 等级: {neighbor[2]}")

    # 计算预测等级
    predicted_class = max(nearest_neighbors, key=lambda x: nearest_neighbors.count(x))[2]
    print(f"  所以knn算法认为<姓名为{test_name}，身高={test_height}m>预测的等级为: {predicted_class}\n")
