import psycopg2
import random

conn = psycopg2.connect(database="postgres",
                        user="kanat",
                        password="kanat123",
                        host="localhost",
                        port="5432")

cur = conn.cursor()

types = {
    1: "trousers",
    2: "jackets",
    3: "three-piece suit",
    4: "two-piece suit",
}

model = {
    1: "skiny",
    2: "classic",
    3: "youth",
    4: "classic giant",
    5: "tuxedo"
}

textile = {
    1: "202 black",
    2: "5129 blue",
    3: "306407",
    4: "5129 gray",
    5: "7901",
    6: "5129 dark gray",
    7: "202 blue atlas",
    8: "209",
    9: "208"
}

height = {
    1: 176,
    2: 182,
    3: 188
}

size_list = {
    1: "44-54",
    2: "50-57",
    3: "46-54",
    4: "58-64"
}


def add_product():
    rand_type = random.randint(1, 4)
    types_ = types[rand_type]
    rand_model = random.randint(1, 5)
    model_ = model[rand_model]
    rand_textile = random.randint(1, 9)
    textile_ = textile[rand_textile]
    rand_height = random.randint(1, 3)
    height_ = height[rand_height]
    rand_size_list = random.randint(1, 4)
    size_list_ = size_list[rand_size_list]
    rand_price = random.randint(80, 120)
    rand_amount = random.randint(500, 1000)

    cur.execute(f"CALL set_product('{types_}', '{model_}', '{textile_}', {height_}, '{size_list_}', {rand_price}, {rand_amount})")


# Генерация информации о заказе
for i in range(100):
    add_product()

conn.commit()

cur.execute("SELECT * FROM sales.product")

rows = cur.fetchall()
for row in rows:
    print(row)

# Закрытие соединения с базой данных
cur.close()
conn.close()
