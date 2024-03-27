import pandas as pd
from itertools import combinations

# 读取交易记录
df = pd.read_excel('data_apriori.xlsx', header=None)

# 获取所有不重复的商品项
unique_items = set()
for index, row in df.iterrows():
    unique_items.update(row.dropna().tolist())


# 定义函数用于生成候选项集
def generate_candidate_itemsets(itemset, k):
    return set(combinations(itemset, k))


# 定义函数用于计算支持度
def calculate_support(itemset):
    count = 0
    for index, row in df.iterrows():
        if all(item in row.values for item in itemset):
            count += 1
    return count / len(df)


# 定义函数用于挖掘频繁项集
def apriori(min_support):
    freq_itemsets = []
    k = 1
    while True:
        candidate_itemsets = generate_candidate_itemsets(unique_items, k)
        freq_itemsets_k = []
        for itemset in candidate_itemsets:
            support = calculate_support(itemset)
            if support >= min_support:
                freq_itemsets_k.append((itemset, support))
        if not freq_itemsets_k:
            break
        freq_itemsets.extend(freq_itemsets_k)
        k += 1
    return freq_itemsets


# 定义函数用于生成关联规则
def generate_association_rules(freq_itemsets, min_confidence):
    association_rules = []
    for itemset, support in freq_itemsets:
        for i in range(1, len(itemset)):
            for subset in combinations(itemset, i):
                left = set(subset)
                right = set(itemset) - left
                confidence = support / calculate_support(left)
                if confidence >= min_confidence:
                    association_rules.append((left, right, support, confidence))
    return association_rules


# 设置支持度和置信度的阈值
min_support_threshold = 0.5
min_confidence_threshold = 0.5

# 挖掘频繁项集
frequent_itemsets = apriori(min_support_threshold)

# 生成关联规则
association_rules = generate_association_rules(frequent_itemsets, min_confidence_threshold)

# 打印频繁项集和关联规则
print("  Frequent Itemsets:")
for itemset, support in frequent_itemsets:
    print(f"  {', '.join(itemset)} - Support: {support:.2f}")

print("\nStrong Association Rules:")
for left, right, support, confidence in association_rules:
    print(f"  {', '.join(left)} -> {', '.join(right)} - Support: {support:.2f}, Confidence: {confidence:.2f}")
