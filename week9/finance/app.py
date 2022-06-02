import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, logout_required

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user = db.execute("SELECT * FROM users WHERE id = ?",
                      session["user_id"])[0]
    rows = db.execute(
        "SELECT * FROM stock_amount WHERE user_id = ?", session["user_id"])

    stock_amount = 0
    purchases = {}

    for row in rows:
        # TODO use concorrency to fetch quotes in parallel
        purchase = row.copy()
        quote = lookup(purchase["symbol"])

        purchase["current_price"] = usd(quote["price"])
        purchase["name"] = quote["name"]

        purchase["total"] = usd(purchase["shares"] * quote["price"])

        stock_amount += quote["price"] * row["shares"]
        purchases[purchase["symbol"]] = purchase

    return render_template("index.html", purchases=purchases.values(),
                           cash=usd(user["cash"]), total=usd(user["cash"] + stock_amount))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares_str = request.form.get("shares")

        if not symbol:
            return apology("missing symbol", 400)
        if not shares_str:
            return apology("missing shares", 400)

        try:
            shares = int(shares_str)
        except:
            return apology("invalid shares value", 400)

        if shares < 0:
            return apology("shares must be positive", 400)

        quote = lookup(symbol)

        if not quote:
            return apology("invalid symbol", 400)

        price = quote["price"]
        amount = shares * price

        rows = db.execute("SELECT cash FROM users WHERE id = ?",
                          session["user_id"])

        balance = rows[0]["cash"]

        if(amount > balance):
            return apology("can't afford", 400)

        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   balance - amount, session["user_id"])

        db.execute("INSERT INTO purchase_history (user_id, symbol, price, shares) VALUES(?, ?, ?, ?)",
                   session["user_id"], quote["symbol"], price, shares)

        db.execute("INSERT INTO stock_amount (user_id, symbol, shares) VALUES (?, ?, ?) ON CONFLICT(user_id, symbol) DO UPDATE SET shares=shares+?",
                   session["user_id"], quote["symbol"], shares, shares)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute(
        "SELECT * FROM purchase_history WHERE user_id = ?", session["user_id"])

    purchases = {}

    for row in rows:
        # TODO use concorrency to fetch quotes in parallel
        purchase = row.copy()
        quote = lookup(purchase["symbol"])

        purchase["current_price"] = usd(quote["price"])
        purchase["transacted_at"] = purchase["created_at"]
        purchases[purchase["symbol"]] = purchase

    return render_template("history.html", purchases=purchases.values())


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("missing symbol", 400)

        quote = lookup(request.form.get("symbol"))

        if not quote:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", quote=quote)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
@logout_required
def register():
    """Register user"""

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("must provide username", 400)
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 400)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password confirmation doesn't match", 400)

        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        if len(rows) != 0:
            return apology("username already registred", 400)

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)",
                   request.form.get("username"),
                   generate_password_hash(request.form.get("password")))

        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/change-password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change password"""
    if request.method == "POST":

        if not request.form.get("old_password"):
            return apology("must provide old password", 403)
        elif not request.form.get("new_password"):
            return apology("must provide new password", 403)

        rows = db.execute("SELECT * FROM users WHERE id = ?",
                          session["user_id"])

        if not check_password_hash(rows[0]["hash"], request.form.get("old_password")):
            return apology("invalid password", 403)

        db.execute("UPDATE users SET hash = ? WHERE id = ?",
                   generate_password_hash(request.form.get("new_password")),
                   session["user_id"])
        return redirect("/")

    return render_template("change-password.html")


@app.route("/deposit-money", methods=["GET", "POST"])
@login_required
def deposit_money():
    """Change password"""
    if request.method == "POST":

        if not request.form.get("password"):
            return apology("must provide password", 403)

        rows = db.execute("SELECT * FROM users WHERE id = ?",
                          session["user_id"])

        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid password", 403)

        amount_str = request.form.get("amount")

        if not amount_str:
            return apology("missing amount", 400)

        try:
            amount = int(amount_str)
        except:
            return apology("invalid amount", 400)

        if amount < 0:
            return apology("amount must be positive", 400)

        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   rows[0]["cash"] + amount,
                   session["user_id"])
        return redirect("/")

    return render_template("deposit-money.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares_str = request.form.get("shares")

        if not symbol:
            return apology("missing symbol", 400)
        if not shares_str:
            return apology("missing shares", 400)

        stock = db.execute(
            "SELECT * FROM stock_amount WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)[0]

        try:
            shares = int(shares_str)
        except:
            return apology("invalid shares value", 400)

        if shares > stock["shares"]:
            return apology("too many shares", 400)

        quote = lookup(symbol)
        amount = shares * quote["price"]

        user = db.execute("SELECT cash FROM users WHERE id = ?",
                          session["user_id"])[0]

        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   user["cash"] + amount, session["user_id"])

        db.execute("INSERT INTO purchase_history (user_id, symbol, price, shares) VALUES(?, ?, ?, ?)",
                   session["user_id"], symbol, quote["price"], -1*shares)

        db.execute("UPDATE stock_amount SET shares = ? WHERE user_id = ? AND symbol = ?",
                   stock["shares"] - shares, session["user_id"], symbol)

        return redirect("/")
    else:
        symbols = db.execute(
            "SELECT symbol FROM stock_amount WHERE user_id = ?", session["user_id"])
        # TODO improve max
        return render_template("sell.html", symbols=symbols, max=5)
