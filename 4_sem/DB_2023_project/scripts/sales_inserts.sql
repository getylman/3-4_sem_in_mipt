insert into sales.product (id_product, type_of_product,
	model_of_product, textile_of_product,
	height_of_product, size_list_of_product,
	price_of_product, amount) values
(1, 'trousers', 'skiny', '202 black', 182, '44-54', 9, 20),
(2, 'trousers', 'skiny', '5129 blue', 176, '44-54', 9, 15),
(3, 'jacket', 'classic', '306407', 176, '50-57', 27, 10),
(4, 'jacket', 'classic', '202 black', 176, '50-57', 25, 5),
(5, 'trousers', 'classic', '315 gray', 176, '50-57', 10, 30),
(6, 'three-piece suit', 'youth', '7901', 182, '46-54', 35, 10),
(7, 'two-piece suit', 'classic giant', '5129 dark gray', 188, '58-64', 30, 5),
(8, 'trousers', 'tuxedo', '202 black atlas', 182, '46-56', 13, 20),
(9, 'two-piece suit', 'tuxedo', '202 blue atlas', 182, '46-56', 31, 10),
(10, 'jacket', 'youth', '306407', 188, '46-54', 25, 10)

insert into sales.client (id_client, surname,
	firstname, telephone, email, card_number, address) values
(1, 'Milos', 'Ricardo', '+7(800)-555-35-35', 'ultratrueman@gmail.com', '1223344556679009', 'Moscow'),
(2, 'Gigachadov', 'Baseman', '+7(913)-123-23-32', 'gigaman@gmail.com', '1223344556679009', 'Moscow'),
(3, 'Yan', 'Chun', '+7(888)-555-35-35', 'greatxi@gmail.com', '1223344556679009', 'Moscow'),
(4, 'Tennison', 'Ben', '+1(101)-010-10-10', 'ben10@gmail.com', '1223344556679009', 'Moscow'),
(5, 'Mr', 'Frost', '+7(813)-555-35-35', 'komunyka3000@gmail.com', '1223344556679009', 'Moscow'),
(6, 'Lolo', 'Loshka', '+7(814)-555-35-35', 'lololoshka@gmail.com', '1223344556679009', 'Moscow'),
(7, 'Patison', 'Robert', '+1(999)-444-35-35', 'a1b1c3@gmail.com', '1223344556679009', 'Moscow'),
(8, 'Milos', 'Anti Ricardo', '+7(800)-555-35-35', 'ultrafalseman@gmail.com', '1223344556679009', 'Moscow'),
(9, 'Gosling', 'Rayon', '+1(800)-555-35-35', 'barbyejoyer@gmail.com', '1223344556679009', 'Moscow'),
(10, 'Anti Milos', 'Ricardo', '+7(800)-555-35-35', 'ultratrueman@gmail.com', '1223344556679009', 'Moscow')

insert into sales.transactions (id_transaction, client_id,
	product_id, amout, order_date) values
(1, 5, 1, 5, '2023-04-05'),
(2, 6, 2, 1, '2023-04-05'),
(3, 1, 1, 2, '2023-04-05'),
(4, 1, 10, 3, '2023-04-05'),
(5, 2, 1, 4, '2023-04-05'),
(6, 6, 1, 5, '2023-04-05'),
(7, 2, 6, 5, '2023-04-05'),
(8, 5, 8, 3, '2023-04-05'),
(9, 5, 9, 1, '2023-04-05'),
(10, 5, 2, 7, '2023-04-05')
