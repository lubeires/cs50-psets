import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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

    stocks = db.execute(
        """
        SELECT symbol, SUM(shares) AS shares FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        """,
        session["user_id"]
    )

    total = 0
    for stock in stocks:
        s = lookup(stock["symbol"])
        stock["name"] = s["name"]
        stock["price"] = s["price"]
        total += (stock["price"] * stock["shares"])

    cash = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    params = {
        "stocks": stocks,
        "cash": cash,
        "total": total + cash
    }

    return render_template("/index.html", params=params)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Checks if any field is blank
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("fill in all fields", 400)

        symbol = request.form.get("symbol")
        stock = lookup(symbol)

        # Checks if symbol is invalid
        if not stock:
            return apology("invalid symbol", 400)

        # Checks if shares is invalid
        if not request.form.get("shares").isnumeric() or int(request.form.get("shares")) < 1:
            return apology("invalid shares", 400)

        shares = int(request.form.get("shares"))

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        price = stock["price"]

        # Checks if cash is enough
        if cash < price * shares:
            return apology("insufficient cash", 400)

        # Updates user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash - (price * shares)), session["user_id"])

        # Insert new purchase into the purchases table
        db.execute("""
                    INSERT INTO transactions (type, symbol, price, shares, date_time, user_id)
                    VALUES ("PURCHASE", upper(?), ?, ?, datetime('now'), ?)
                   """,
                   symbol, price, shares, session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute(
        """
        SELECT * FROM transactions
        ORDER BY date_time DESC
        """)

    return render_template("/history.html", transactions=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
        # Checks if a symbol was provided
        if not request.form.get("symbol"):
            return apology("provide a symbol", 400)

        stock = lookup(request.form.get("symbol"))

        # Checks if symbol is valid
        if not stock:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", stock=stock)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Checks if any field is blank
        if not request.form.get("username") or not request.form.get("password") or not request.form.get("confirmation"):
            return apology("fill in all fields", 400)

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        usernameExists = db.execute("SELECT COUNT(*) AS c FROM users WHERE username = ?", username)[0]["c"]

        # Checks if username already exists
        if usernameExists:
            return apology("username not available", 400)

        # Checks if password did not match confirmation
        elif password != confirmation:
            return apology("password did not match", 400)

        # Insert new user into database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    stocks = db.execute(
        """
        SELECT symbol, SUM(shares) AS shares FROM transactions
        WHERE user_id = ? AND type = "PURCHASE"
        GROUP BY symbol
        """,
        session["user_id"]
    )

    symbols = []
    for stock in stocks:
        symbols.append(stock["symbol"])

    if request.method == "POST":

        # Checks if any field is blank
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("fill in all fields", 400)

        symbol = str(request.form.get("symbol")).strip()

        # Checks if symbol is valid
        if symbol not in symbols:
            return apology("invalid symbol", 400)

        symbol = str(symbol).strip()

        # Gets maximum value for shares
        for stock in stocks:
            if stock["symbol"] == symbol:
                sharesAvailable = stock["shares"]

        # Checks if shares is valid
        if not request.form.get("shares").isnumeric():
            return apology("invalid shares", 400)

        shares = int(request.form.get("shares"))

        if shares < 1 or shares > sharesAvailable:
            return apology("invalid shares", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        stock = lookup(symbol)
        price = stock["price"]

        # Updates user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash + (price * shares)), session["user_id"])

        # Insert new sale into the sales table
        db.execute("""
                    INSERT INTO transactions (type, symbol, price, shares, date_time, user_id)
                    VALUES ("SALE", upper(?), ?, ?, datetime('now'), ?)
                   """,
                   symbol, price, -shares, session["user_id"])

        return redirect("/")
    else:
        return render_template("sell.html", symbols=symbols)