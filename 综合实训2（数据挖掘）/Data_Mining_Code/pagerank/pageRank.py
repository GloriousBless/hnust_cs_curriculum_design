import numpy as np


def pagerank(adj_matrix, damping=0.85, epsilon=1.0e-3):
    # 转换邻接矩阵为概率转移矩阵
    out_degree = np.sum(adj_matrix, axis=1)
    prob_matrix = adj_matrix / np.where(out_degree[:, None] == 0, 1, out_degree[:, None])

    num_pages = len(adj_matrix)
    teleport = np.ones((num_pages, num_pages)) / num_pages
    pagerank_vector = np.ones(num_pages)

    while True:
        new_pagerank = (1 - damping) * np.dot(teleport, pagerank_vector) + damping * np.dot(prob_matrix.T, pagerank_vector)  # 公式计算
        if np.linalg.norm(new_pagerank - pagerank_vector, ord=1) < epsilon:  # 迭代终止条件
            break
        pagerank_vector = new_pagerank

    return pagerank_vector


# 邻接矩阵
adjacency_matrix = np.array([
    [0, 1, 1, 1],
    [1, 0, 0, 1],
    [1, 0, 0, 0],
    [0, 1, 1, 0]
])

# 计算PageRank
result = pagerank(adjacency_matrix)

# 对PageRank结果进行排序并输出网页重要性
sorted_pages = sorted(enumerate(result), key=lambda x: x[1], reverse=True)
print("  网页重要性排序：")
for page, score in sorted_pages:
    letter_page = chr(ord('A') + page)  # 将数字转换为字母，A对应0，B对应1，依次类推
    print(f"  Page {letter_page}: PageRank = {score:.4f}")
