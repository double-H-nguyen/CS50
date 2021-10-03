import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    user = db.execute("SELECT username, cash FROM users WHERE id=?", user_id)[0]
    username = user["username"]
    cash = user["cash"]
    
    # Query stocks that user owns from ownership table
    stocks = db.execute("SELECT symbol, name, shares FROM ownership WHERE user_id=? ORDER BY symbol ASC", user_id)

    # Add current stock price and total value to stock dictionary
    for stock in stocks:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["total_value"] = stock["price"] * stock["shares"]
    # * Stock data structure {'symbol': _, 'name': _, 'shares': _, 'price':_, 'total_value': _}

    # Calculate grand total value
    grand_total = 0
    grand_total += cash
    for stock in stocks:
        grand_total += stock["total_value"]
    
    return render_template("index.html", username=username, cash=cash, grand_total=grand_total, stocks=stocks)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # POST
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Symbol cannot be blank")
        if not lookup(symbol):
            return apology("Symbol does not exist")
        
        if not shares:
            return apology("Shares cannot be blank")
        shares = int(shares)
        if shares <= 0:
            return apology("Must enter 1 or more shares")

        stock_dict = lookup(symbol)
        user_id = session["user_id"]

        stock_price_per_share = stock_dict["price"]
        cash_available = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"]
        remaining_cash = cash_available - (stock_price_per_share * shares)
        if remaining_cash < 0:
            return apology("You do not have enough funds to buy the current stock amount")

        # Insert transaction in transactions table
        db.execute("INSERT INTO transactions (user_id,type,name,symbol,price,shares) VALUES (?,?,?,?,?,?)", user_id, "buy", stock_dict["name"], stock_dict["symbol"], stock_price_per_share, shares)

        # Update cash amount in users table
        db.execute("UPDATE users SET cash=? WHERE id=?", remaining_cash, user_id)

        # Insert/Update stock in ownership table
        ownership_shares_row = db.execute("SELECT shares FROM ownership WHERE user_id=? AND symbol=?", user_id, stock_dict["symbol"])
        if not ownership_shares_row: # If user doesn't currently own the stock, insert into table with shares bought
            db.execute("INSERT INTO ownership (user_id, name, symbol, shares) VALUES (?,?,?,?)", user_id, stock_dict["name"], stock_dict["symbol"], shares)
        else: # update shares of existing stock
            ownership_shares = ownership_shares_row[0]["shares"]
            ownership_shares += shares 
            db.execute("UPDATE ownership SET shares=? WHERE user_id=? AND symbol=?", ownership_shares, user_id, stock_dict["symbol"])

        # Redirect to the index page
        return redirect("/")

    # GET
    return render_template("buy.html")


# * This route is specifically for using AJAX to query for the stock info
@app.route("/search")
@login_required
def search():
    symbol = request.args.get("q")
    stock_dict = lookup(symbol)
    return jsonify(stock_dict)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


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
        symbol = request.form.get("symbol")
        symbol_dict = lookup(symbol)
        return render_template("quoted.html", stock_name=symbol_dict["name"], stock_symbol=symbol_dict["symbol"], stock_price=symbol_dict["price"])

    # GET
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check if username is blank
        if not username:
            return apology("Username cannot be blank")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Check if username already exists
        if len(rows) > 0:
            return apology("Username already exist")

        # Check if password or confirmation password is blank
        if not password or not confirmation:
            return apology("Password and/or confirmation input is blank")

        # Check if password match
        if password != confirmation:
            return apology("Passwords do not match")

        # Hash password
        password = generate_password_hash(password)

        # INSERT user into users table (finance.db)
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, password)

        # Automatically log user, and redirect to index()
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]
        return redirect("/")

    # GET method
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
