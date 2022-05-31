# week 9

## birthday

### sqlite

```sh
sqlite3 birthdays.db 
.schema
select * from birthdays;
```

### layout.html

```html
<!DOCTYPE html>

<html lang="en">

<head>
    <link href="https://fonts.googleapis.com/css2?family=Montserrat:wght@500&display=swap" rel="stylesheet">
    <link href="/static/styles.css" rel="stylesheet">
    <title>Birthdays</title>
</head>

<body>
    {% block body %}{% endblock %}
</body>

</html>
```

### index.html

```html
{% extends "layout.html" %}
{% block body %}
<div class="header">
    <h1>Birthdays</h1>
</div>
<div class="container">
    <div class="section">
        <h2>Add a Birthday</h2>

        <form action="/" method="post">
            <input autocomplete="off" autofocus name="name" placeholder="Name" type="text">
            <input autocomplete="off" name="month" placeholder="Month" type="number">
            <input autocomplete="off" name="day" placeholder="Day" type="number">

            <input type="submit" value="Add Birthday">
        </form>
    </div>

    <div class="section">

        <h2>All Birthdays</h2>
        <table>
            <thead>
                <tr>
                    <th>Name</th>
                    <th>Birthday</th>
                </tr>
            </thead>
            <tbody>
                {% for birthday in birthdays %}
                <tr>
                    <td>{{ birthday["name"] }}</td>
                    <td>{{ birthday["month"] }}/{{ birthday["day"] }}</td>
                    <td>
                        <form action="/delete" method="post">
                            <input name="id" type="hidden" value="{{ birthday.id }}">
                            <input type="submit" value="Delete">
                        </form>
                    </td>
                </tr>
                {% endfor %}
            </tbody>
        </table>
    </div>
</div>
{% endblock %}
```

### Optionally, you may also

Add the ability to delete and/or edit birthday entries.
Add any additional features of your choosing!

## finance

https://finance.cs50.net/

```sql
CREATE TABLE stock_amount (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id),
    UNIQUE(user_id, symbol)
);

-- queries
SELECT * FROM stock_amount;
INSERT INTO stock_amount (user_id, symbol, shares) VALUES (1, "VOO", 4) ON CONFLICT(user_id, symbol) DO UPDATE SET shares=shares+4;
```

```sql
CREATE TABLE purchase_history (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    user_id INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    price DECIMAL(5,2) NOT NULL,
    shares INTEGER NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id)
);
```

TODO:

- delete table purchases
- create new table wallet (id, user_id, symbol, shares) with index user+symbol and foreign key
- create history table with all transactions and
