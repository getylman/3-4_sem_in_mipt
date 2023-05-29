select sales.product.model_of_product from sales.product
group by id_product, model_of_product  
having model_of_product = 'tuxedo'

select * from sales.product
group by id_product, type_of_product, size_list_of_product
order by amount

select id_client, surname, firstname, telephone, email, count(*) over (order by id_client) as order_count_rank
from sales.client;

select email, count(*) over (partition by email) / count(*) over () * 100 as percent_of_total
from sales.client;

select id_product, model_of_product, size_list_of_product,
count(size_list_of_product) over (order by size_list_of_product) as size_count_rank
from sales.product;

select textile_of_product,
count(*) over (partition by textile_of_product) / count(*) over() * 100 as percent_of_total
from sales.product;

select type_of_product, avg(price_of_product) over (partition by type_of_product) as avg_price
from sales.product;

insert into sales.transactions (id_transaction, client_id,
	product_id, amout, order_date) values
(15, 7, 9, 1, '2077-01-23')

delete from sales.client
where id_client = 10

update sales.client 
	set telephone = '+5(100)-999-01-10'
where firstname = 'Ricardo'
