from flask import Flask, redirect, render_template, request, session
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
from flask_session import Session
from tempfile import mkdtemp
from datetime import datetime
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import login_required, error, add_goal_validation, update_goal_validation, login_validation


#*******************************************
#* Application Configuration
#*******************************************
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


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


#*******************************************
#* Database Configuration
#*******************************************
# Initialize database - https://flask-sqlalchemy.palletsprojects.com/en/2.x/quickstart/#a-minimal-application
# Create initial database with the following python commands
# 'python'
# 'from application import db'
# 'db.create_all()'
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///goal_tracker.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)
migrate = Migrate(app, db)


class Users(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  username = db.Column(db.Text, nullable=False)
  password = db.Column(db.Text, nullable=False)
  goals = db.relationship('Goals', backref='users', lazy=True)

  def __repr__(self):
    return f'<User username="{self.username}", goals(length)={len(self.goals)}>'


class Goals(db.Model):
  id = db.Column(db.Integer, primary_key=True)
  title = db.Column(db.Text, nullable=False)
  description = db.Column(db.Text, nullable=True)
  num_of_completions_required = db.Column(db.Integer, nullable=False)
  reward = db.Column(db.Text, nullable=False)
  num_of_completed = db.Column(db.Integer, default=0, nullable=False)
  is_completed = db.Column(db.Boolean, default=False, nullable=False)
  date_created = db.Column(db.DateTime, default=datetime.utcnow, nullable=False)
  user_id = db.Column(db.Integer, db.ForeignKey('users.id'), nullable=False)

  def __repr__(self):
    return f'<User id={self.id}, title="{self.title}", description="{self.description}", num_of_completions_required={self.num_of_completions_required}, reward="{self.reward}", num_of_completed="{self.num_of_completed}" date_created={self.date_created}, user_id={self.user_id}>'


#*******************************************
#* Routes
#*******************************************
@app.route('/')
@login_required
def index():
  user_id = session["user_id"]
  
  # Query goals
  goals = Goals.query.filter_by(user_id=user_id)
  return render_template("index.html", goals=goals)


@app.route('/add_goal', methods=['GET', 'POST'])
@login_required
def add_goal():
  user_id = session["user_id"]
  if request.method == "POST":
    title = request.form.get('title')
    description = request.form.get('description')
    completions_required = request.form.get('completions_required')
    reward = request.form.get('reward')

    # Validation
    is_valid, msg = add_goal_validation(title, description, completions_required, reward)
    if not is_valid:
      return error(msg)

    # Add goal to database
    new_goal = Goals(title=title, description=description, num_of_completions_required=completions_required, reward=reward, user_id=user_id)
    try:
      db.session.add(new_goal)
      db.session.commit()
      return redirect('/')
    except:
      return error("Failed to add goal to database")

  return render_template("add_goal.html")


@app.route('/update_goal/<int:id>', methods=['GET', 'POST'])
@login_required
def update_goal(id):
  user_id = session["user_id"]
  # Query goal (if it exist)
  goal = Goals.query.filter_by(id=id, user_id=user_id).first_or_404() 

  if request.method == "POST":
    title = request.form.get('title')
    description = request.form.get('description')
    completions_required = request.form.get('completions_required')
    num_of_completed = request.form.get('times_completed')
    reward = request.form.get('reward')

    # Validation
    is_valid, msg = update_goal_validation(title, description, completions_required, num_of_completed, reward)
    if not is_valid:
      return error(msg)

    # Update goal to database
    goal.title = title
    goal.description = description
    goal.num_of_completions_required = completions_required
    goal.num_of_completed = num_of_completed
    goal.reward = reward

    # Check if checkbox was checked
    if request.form.get('is_completed'):
      goal.is_completed = True     
    else: # form returned NONE
      goal.is_completed = False  

    try:
      db.session.commit()
      return redirect('/')
    except:
      return error('Failed to update goal')

  return render_template("update_goal.html", goal=goal)


@app.route('/delete_goal/<int:id>', methods=['GET', 'POST'])
@login_required
def delete_goal(id):
  user_id = session["user_id"]
  goal = Goals.query.filter_by(id=id, user_id=user_id).first_or_404() 

  if request.method == "POST":
    try:
      db.session.delete(goal)
      db.session.commit()
      return redirect('/')
    except:
      return error('Failed to delete goal')

  return render_template("delete_goal.html", goal=goal)


@app.route('/increment/<int:id>')
@login_required
def increment(id):
  user_id = session["user_id"]
  goal = Goals.query.filter_by(id=id, user_id=user_id).first_or_404()

  if goal.is_completed:
    return error('Goal was already marked complete')
  else:
    goal.num_of_completed += 1

  # If completions required has been reached, set the goal completion status
  if goal.num_of_completed == goal.num_of_completions_required:
    goal.is_completed = True

  try:
    db.session.commit()
    return redirect('/')
  except:
    return error('Failed to increment goal')


@app.route('/login', methods=['GET', 'POST'])
def login():
  if request.method == "POST":
    username = request.form.get('username')
    password = request.form.get('password')

    # Validation
    is_valid, msg = login_validation(username, password)
    if not is_valid:
      return error(msg)

    # Query database for username
    user = Users.query.filter_by(username=username).first()

    # Check if username and password are correct
    if (not user) or (not check_password_hash(user.password, password)):
      return error('Username or password was incorrect')

    # Remember which user has logged in
    session["user_id"] = user.id

    # Redirect user to home page
    return redirect("/")
  
  # GET
  return render_template("login.html")


@app.route('/logout')
def logout():
  # Forget any user_id
  session.clear()

  # Redirect user to login form
  return redirect('/login')


@app.route('/register', methods=['GET', 'POST'])
def register():
  if request.method == "POST":
    username_input = request.form.get('username')
    password_input = request.form.get('password')
    confirmation_input = request.form.get('confirmation')
    #TODO: input validation from helper function

    # Hash password
    hashed_password = generate_password_hash(password_input)

    # Add to database
    new_user = Users(username=username_input, password=hashed_password)
    try:
      db.session.add(new_user)
      db.session.commit()
      return redirect('/')
    except:
      return error("Registration was unsuccessful")

  # GET
  return render_template("register.html")


@app.route('/account')
@login_required
def account():
  user_id = session["user_id"]
  user = Users.query.get_or_404(user_id) 
  return render_template ("account.html", username=user.username)


@app.route('/change_password', methods=['GET', 'POST'])
@login_required
def change_password():
  user_id = session["user_id"]
  user = Users.query.get_or_404(user_id) 

  if request.method == "POST":
    original_password_input = request.form.get('original')
    new_password_input = request.form.get('new')
    confirmation_password_input = request.form.get('confirmation') 
    #TODO: input validation from helper function

    # Check original password is correct
    valid_password = check_password_hash(user.password, original_password_input)
    if not valid_password:
      return error("Original password was incorrect")

    # Hash password
    hashed_password = generate_password_hash(new_password_input)

    # Update user's password
    user.password = hashed_password  
    try:
      db.session.commit()
      return redirect('/')
    except:
      return error("Unable to change password")

  # GET
  return render_template("change_password.html")


@app.route('/delete_account', methods=['GET', 'POST'])
@login_required
def delete_account():
  user_id = session["user_id"]
  user = Users.query.get_or_404(user_id) 
  goal_ct = int(Goals.query.filter_by(user_id=user_id).count())

  if request.method == "POST":
    password_input = request.form.get('password')
    #TODO: input validation from helper function
    # Check original password is correct
    valid_password = check_password_hash(user.password, password_input)
    if not valid_password:
      return error("Password was incorrect")

    # Delete user
    try:
      # IMPORTANT: Delete user's goals before deleting user
      Goals.query.filter_by(user_id=user.id).delete()
      db.session.delete(user)
      db.session.commit()
      return redirect('/logout')
    except:
      return error("Failed to delete goal")

  # GET
  return render_template("delete_account.html", id=user.id, username=user.username, goal_ct=goal_ct)


def errorhandler(e):
  """Handle error"""
  if not isinstance(e, HTTPException):
    e = InternalServerError()
  return error(e.name, e.code)


# Listen for errors
for code in default_exceptions:
  app.errorhandler(code)(errorhandler)


if __name__ == "__main__":
  db.create_all() # create tables if they haven't already been created
  app.run(debug=True) # debug mode enabled