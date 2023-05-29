create schema if not exists sales

create table if not exists sales.product (
  id_product int not null primary key,
  type_of_product text not null,
  model_of_product text not null,
  textile_of_product text not null,
  height_of_product int not null,
  size_list_of_product text not null,
  price_of_product numeric not null,
  amount numeric not null
)

create table if not exists sales.client (
  id_client int not null primary key,
  surname text not null,
  firstname text not null,
  telephone text not null,
  email text not null,
  card_number text not null,
  address text not null
)

create table if not exists sales.transactions (
  id_transaction int not null primary key,
  client_id int not null,
    foreign key (client_id) references
    sales.client(id_client) on delete set null,
  product_id int not null,
    foreign key (product_id) references
    sales.product(id_product) on delete set null,
  amout numeric not null,
  order_date date
)

create table if not exists client_budget (
  card_number text primary key,
  budget numeric
);
