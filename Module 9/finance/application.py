import os

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

#Collects date and time
date = str(datetime.now())

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd
app.jinja_env.filters["lookup"] = lookup

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

    user_id = session['user_id']
    user_stocks = db.execute("SELECT * FROM stocks WHERE UserId=? AND shares > 0", user_id)
    user_account = db.execute("SELECT * FROM users WHERE id=?", user_id)
    network = user_account[0]['cash']
    for i in user_stocks:
        network = network + ((lookup(i['symbol'])['price'])*i['shares'])
    return render_template("index.html", userInf=user_account, stocksInf=user_stocks, network=network)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        stock = lookup(request.form.get("symbol"))
        shares = int(request.form.get("shares"))
        balance = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])

        if not request.form.get("symbol") or not stock:
            return apology("Symbol invalid or missing", 400)

        if shares < 1 or not isinstance(shares, int):
            return apology("Negative shares number", 400)

        if balance[0]['cash'] < (shares * stock['price']):
            return apology("Insufficient balance", 400)

        if db.execute("SELECT shares FROM stocks WHERE UserId=? AND symbol=?", session["user_id"], stock["symbol"]):
            db.execute("UPDATE stocks SET shares = shares+? WHERE UserId=? AND symbol=?", shares, session["user_id"],stock["symbol"])
            db.execute("UPDATE users SET cash = cash-? WHERE id=?", (shares * stock['price']), session['user_id'])

        else:
            db.execute("INSERT INTO stocks VALUES (?,?,?)", stock["symbol"], shares, session["user_id"] )
            db.execute("UPDATE users SET cash = cash-? WHERE id=?", (shares * stock['price']), session['user_id'])

        db.execute("INSERT INTO history VALUES ('buy',?,?,?,?,?)", stock["symbol"], shares, session["user_id"], date, stock['price'])
        return redirect("/")

    if request.method == "GET":
        return render_template("buy.html")

    return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM history WHERE UserId = ?", session["user_id"])
    return render_template("history.html", history=history)


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

        stocksymbol = request.form.get("symbol")
        stksym = lookup(stocksymbol)

        #Checks if stocksymbol has a value
        if not stocksymbol or not stksym or len(stksym) != 3:
            return apology("Invald stock symbol or not found", 400)


        return render_template("quoted.html", stksym=stksym)

    if request.method == "GET":

        #Display a page waiting for an stock symbol
        return render_template("quote.html")

    return apology("TODO")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        #Check if the user's username is null or already exists
        if len(rows) != 0 or not request.form.get("username"):
            return apology("Invalid username", 400)

        #Check if the user has provided a password
        if not request.form.get("password"):
            return apology("Must provide password", 400)

        #Check if passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords don't match", 400)

        #Adds the inputs to the database
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session['user_id']
    user_stocks = db.execute("SELECT * FROM stocks WHERE UserId=? AND shares > 0", user_id)

    if request.method == "POST":
        firm = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        usershares = db.execute("SELECT shares FROM stocks WHERE symbol = ? AND UserId= ?", firm, user_id)

        if not shares or shares <= 0 or not isinstance(shares, int):
            return apology("Invalid shares number", 400)

        if not firm or not usershares:
            return apology("Invalid symbol", 400)

        if db.execute("SELECT shares FROM stocks WHERE UserId = ? AND symbol = ?", session['user_id'], firm)[0]['shares'] < shares:
            return apology("Not enough shares", 400)

        db.execute("UPDATE stocks SET shares = shares - ? WHERE UserId = ? AND symbol = ?",shares, user_id, firm)
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", lookup(firm)['price']*shares, user_id)
        db.execute("INSERT INTO history VALUES ('sell',?,?,?,?,?)", firm, shares, user_id, date, lookup(firm)['price'])

        return redirect("/")

    if request.method == "GET":
        return render_template("sell.html", stocksInf=user_stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
