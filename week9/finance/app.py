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
    rows = db.execute(
        "SELECT * FROM purchases WHERE user_id = ?", session["user_id"])

    stock_amount = 0
    purchases = {}

    for row in rows:
        quote = lookup(row["symbol"])

        row["current_price"] = usd(quote["price"])
        row["name"] = quote["name"]

        if purchases.get(row["symbol"]):
            purchases[row["symbol"]]["shares"] += row["shares"]
        else:
            purchases[row["symbol"]] = row

        purchases[row["symbol"]]["total"] = usd(
            purchases[row["symbol"]]["shares"] * quote["price"])

        stock_amount += quote["price"] * row["shares"]

    user = db.execute("SELECT * FROM users WHERE id = ?",
                      session["user_id"])[0]

    return render_template("index.html", purchases=purchases.values(),
                           cash=user["cash"], stock_amount=stock_amount)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        if not request.form.get("shares"):
            return apology("missing shares", 400)

        shares = int(request.form.get("shares"))
        if shares < 0:
            return apology("shares must be positive", 400)

        quote = lookup(request.form.get("symbol"))

        if not quote:
            return apology("invalid symbol", 400)

        amount = shares * quote["price"]

        rows = db.execute("SELECT cash FROM users WHERE id = ?",
                          session["user_id"])

        balance = rows[0]["cash"]

        print(shares, quote["price"])

        if(amount > balance):
            return apology("can't afford", 400)

        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   balance - amount, session["user_id"])

        db.execute("INSERT INTO purchases (user_id, symbol, price, shares) VALUES(?, ?, ?, ?)",
                   session["user_id"], quote["symbol"], quote["price"], shares)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO history")


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
            return apology("must provide username", 403)
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 403)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("password confirmation doesn't match", 400)

        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        if len(rows) != 0:
            return apology("username already registred", 409)

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)",
                   request.form.get("username"),
                   generate_password_hash(request.form.get("password")))

        return redirect("/login", code=307)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO sell")
