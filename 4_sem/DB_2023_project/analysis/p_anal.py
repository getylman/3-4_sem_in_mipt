import psycopg2
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

conn = psycopg2.connect(database="postgres",
                        user="kanat",
                        password="kanat123",
                        host="localhost",
                        port="5432")

cur = conn.cursor()

# Загрузка данных из БД
data = pd.read_sql_query("SELECT * FROM sales.product ", conn)

# Вывод первых 5 строк таблицы
print(data.head())

# Описание данных
print(data.describe())

# Количество уникальных значений в столбце
print(data['model_of_product'].nunique())

# Группировка данных и подсчет количества
grouped_data = data.groupby('model_of_product').size().reset_index(name='counts')
print(grouped_data)

# Гистограмма распределения данных
sns.histplot(data['model_of_product'], kde=False)

# Ящик с усами
sns.boxplot(x='model_of_product', y='price_of_product', data=data)

# Диаграмма рассеяния
sns.scatterplot(x='model_of_product', y='price_of_product', data=data)

# Круговая диаграмма
grouped_data.plot(kind='pie', y='counts', labels=grouped_data['model_of_product'], autopct='%1.1f%%')

# Линейный график
data.plot(x="type_of_product", y=["price_of_product"])
plt.show()
