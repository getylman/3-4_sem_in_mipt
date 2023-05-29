CREATE OR REPLACE PROCEDURE set_product(
    IN type_ TEXT,
    IN model TEXT,
    IN textile TEXT,
    IN height INT,
    IN size_list TEXT,
    IN price NUMERIC,
    IN amount INT
)
LANGUAGE plpgsql
AS $$
DECLARE
    product_id INT;
BEGIN
    SELECT id_product INTO product_id FROM sales.product WHERE 
        type_of_product = type_ AND 
        model_of_product = model AND 
        textile_of_product = textile AND 
        height_of_product = height AND 
        size_list_of_product = size_list AND 
        price_of_product = price LIMIT 1;
    
    IF product_id IS NOT NULL THEN
        UPDATE sales.product SET amount = amount + amount WHERE id_product = product_id;
    ELSE
        INSERT INTO sales.product (type_of_product, model_of_product, textile_of_product, height_of_product, size_list_of_product, price_of_product, amount)
        VALUES (type_, model, textile, height, size_list, price, amount);
    END IF;
END;
$$;

--процедура для добавления товара в таблицу с товаром при наличии такого товара то мы просто увеличиваем его количесво

CREATE FUNCTION get_id_of_product(unit_type TEXT, unit_model TEXT,
  unit_textile TEXT, unit_height TEXT, unit_size_list TEXT,
  unit_price NUMERIC)
RETURNS INT AS $$
DECLARE
  id_of_product INT;
BEGIN
  SELECT id_product INTO id_of_product FROM sales.product
  WHERE type_of_product = $1 AND model_of_product = $2
                 AND textile_of_product = $3
                 AND height_of_product = $4
                 AND size_list_of_product = $5
                 AND price_of_product = $6;
  IF id_of_product IS NULL THEN
    RETURN 0;
  ELSE
    RETURN id_of_product;
  END IF;
END;
$$ LANGUAGE plpgsql;

--эта функция для получения id товара для совершения транзакции в случае отсутвия товара функция вернёт ноль


CREATE OR REPLACE FUNCTION get_client_id(
  p_surname TEXT, 
  p_firstname TEXT, 
  p_telephone TEXT, 
  p_email TEXT, 
  p_address TEXT
) RETURNS INT AS $$
DECLARE
  v_id_client INT;
  v_card_number TEXT;
  v_budget NUMERIC;
BEGIN
  -- Проверяем, есть ли клиент в таблице
  SELECT id_client INTO v_id_client FROM sales.client 
  WHERE surname = p_surname 
    AND firstname = p_firstname 
    AND telephone = p_telephone 
    AND email = p_email 
    AND address = p_address;
  
  -- Если клиент не найден, то создаем нового
  IF v_id_client IS NULL THEN
    SELECT MIN(generated_id) INTO v_id_client FROM 
      generate_series(1, 1000000) AS generated_id 
      EXCEPT SELECT id_client FROM sales.client;
    
    -- Генерируем уникальный номер карты
    SELECT lpad(floor(random() * 10000000000000000)::text, 16, '0') 
    INTO v_card_number FROM sales.client_budget 
      WHERE v_card_number NOT IN (SELECT card_number FROM sales.client_budget);

    -- Добавляем нового клиента в таблицу client
    INSERT INTO sales.client (id_client, surname, firstname, telephone, email, card_number, address) 
    VALUES (v_id_client, p_surname, p_firstname, p_telephone, p_email, v_card_number, p_address);
    
    -- Добавляем нового клиента в таблицу client_budget
    SELECT floor(random() * 1000 + 100) INTO v_budget;
    INSERT INTO sales.client_budget (card_number, budget) 
    VALUES (v_card_number, v_budget);
  END IF;
  
  RETURN v_id_client;
END;
$$ LANGUAGE plpgsql;

--1. Проверить, существует ли клиент в таблице client с заданными аргументами. Для этого мы будем искать клиента по всем переданным параметрам, используя оператор AND. Если клиент найден, то возвращаем его id_client.

--2. Если клиента не нашли, то ищем минимальное натуральное число, которое еще не встречалось в таблице client в столбце id_client. Для этого мы можем использовать функцию generate_series() в сочетании с оператором EXCEPT. 

--3. Создаем нового клиента в таблице client, используя INSERT INTO. Генерируем случайный номер карты, состоящий из 16 цифр, не начинающихся с нуля, и не совпадающих ни с одним номером карты в таблице client_budget. Для этого мы можем использовать функцию lpad() в сочетании с функцией random().

--4. Добавляем новую запись в таблицу client_budget с сгенерированным номером карты и случайным бюджетом больше 100.

--5. Возвращаем id_client только что созданного клиента.

CREATE OR REPLACE FUNCTION get_budget(p_card_number TEXT) RETURNS NUMERIC AS $$
DECLARE
  v_budget NUMERIC;
BEGIN
  -- Ищем запись в таблице client_budget
  SELECT budget INTO v_budget FROM client_budget WHERE card_number = p_card_number;
  
  -- Если запись не найдена, то возвращаем 0
  IF v_budget IS NULL THEN
    RETURN 0;
  ELSE
    RETURN v_budget;
  END IF;
END;
$$ LANGUAGE plpgsql;

--Для создания функции get_budget мы можем использовать простой SQL-запрос, который будет искать запись в таблице client_budget по заданному card_number и возвращать соответствующее значение budget. Если запись не найдена, то возвращаем 0.


CREATE OR REPLACE FUNCTION make_transaction(
    surname text,
    firstname text,
    telephone text,
    email text,
    address text,
    type_of_product text,
    model_of_product text,
    textile_of_product text,
    height_of_product integer,
    size_list_of_product text,
    needed_amount integer
) RETURNS integer AS $$
DECLARE
    id_p integer;
    id_c integer;
    card_number text;
    c_budget integer;
    product_price integer;
BEGIN
    -- Получаем id продукта по параметрам
    SELECT get_id_of_product(type_of_product, model_of_product, textile_of_product, height_of_product, size_list_of_product) INTO id_p;

    -- Проверяем, что id продукта не равен 0
    IF id_p = 0 THEN
        RETURN 1;
    END IF;

    -- Проверяем, что количество продукта больше или равно нужному количеству
    SELECT amount, price_of_product INTO product_price, id_p FROM product WHERE id_product = id_p;
    IF product_price < needed_amount THEN
        RETURN 2;
    END IF;

    -- Получаем id клиента по параметрам
    SELECT get_client_id(surname, firstname, telephone, email, address) INTO id_c;

    -- Получаем бюджет клиента
    SELECT card_number INTO card_number FROM client WHERE id_client = id_c;
    SELECT get_budget(card_number) INTO c_budget;

    -- Проверяем, что бюджет клиента больше или равен цене продукта умноженной на нужное количество
    IF c_budget < product_price * needed_amount THEN
        RETURN 3;
    END IF;

    -- Вычитаем нужное количество продукта из таблицы product
    UPDATE product SET amount = amount - needed_amount WHERE id_product = id_p;
    
    -- Вычитаем цену продукта умноженную на нужное количество из бюджета клиента
    UPDATE client_budget SET budget = budget - product_price * needed_amount WHERE card_number = card_number;

    -- Добавляем новую строку в таблицу transactions
    INSERT INTO transactions (id_transaction, client_id, product_id, amount, order_date)
    SELECT COALESCE(MIN(id_transaction), 0) + 1, id_c, id_p, needed_amount, current_date FROM transactions;

    -- Возвращаем 0, чтобы показать, что функция завершилась успешно
    RETURN 0;
END;
$$ LANGUAGE plpgsql;

--1. Функция получает параметры surname, firstname, telephone, email, address, type_of_product, model_of_product, textile_of_product, height_of_product, size_list_of_product, needed_amount.
--2. Функция вызывает функцию get_id_of_product для получения id продукта по параметрам type_of_product, model_of_product, textile_of_product, height_of_product, size_list_of_product. Если id продукта равен 0, функция возвращает 1 и завершает работу.
--3. Функция проверяет, что количество продукта больше или равно нужному количеству.
--4. Функция вызывает функцию get_client_id для получения id клиента по параметрам surname, firstname, telephone, email, address.
--5. Функция получает card_number клиента и его бюджет из таблиц client и client_budget.
--6. Функция проверяет, что бюджет клиента больше или равен цене продукта умноженной на нужное количество.
--7. Функция вычитает нужное количество продукта из таблицы product.
--8. Функция вычитает цену продукта умноженную на нужное количество из бюджета клиента в таблице client_budget.
--9. Функция добавляет новую строку в таблицу transactions.
--10. Функция возвращает 0, чтобы показать, что она завершилась успешно.

CREATE OR REPLACE FUNCTION update_product_amount()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' OR TG_OP = 'UPDATE' THEN
        UPDATE sales.product 
        SET amount = amount - NEW.amount 
        WHERE id_product = NEW.product_id;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER update_product_amount_trigger
AFTER INSERT OR UPDATE ON sales.transactions
FOR EACH ROW
EXECUTE FUNCTION update_product_amount();

--Пояснение: данный триггер создан для автоматического обновления количества товаров в таблице product после каждой транзакции. Триггер выполняет запрос на уменьшение количества товаров в таблице product на количество товаров, которые были проданы в рамках данной транзакции. Это позволяет автоматически отслеживать остатки товаров и избежать ситуации, когда продается больше товаров, чем есть на складе.


CREATE OR REPLACE FUNCTION update_client_discount()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_OP = 'INSERT' OR TG_OP = 'UPDATE' THEN
        UPDATE sales.client 
        SET discount = CASE 
            WHEN (SELECT SUM(amount) FROM sales.transactions WHERE client_id = NEW.client_id) > 1000 THEN 10 
            WHEN (SELECT SUM(amount) FROM sales.transactions WHERE client_id = NEW.client_id) > 500 THEN 5 
            ELSE 0 
            END 
        WHERE id_client = NEW.client_id;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER update_client_discount_trigger
AFTER INSERT OR UPDATE ON sales.transactions
FOR EACH ROW
EXECUTE FUNCTION update_client_discount();

--Пояснение: данный триггер создан для автоматического обновления скидок клиентов после каждой транзакции. Триггер выполняет запрос на обновление скидки клиента в таблице client на основе суммы всех его покупок. Если сумма покупок превышает 1000, то клиент получает скидку 10%, если сумма покупок от 500 до 1000, то клиент получает скидку 5%, в противном случае скидка равна 0%. Это позволяет автоматически отслеживать скидки клиентов и управлять ими в зависимости от их активности.
