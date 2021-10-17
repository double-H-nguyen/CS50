from flask import Flask, redirect, render_template, request, session
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
from flask_session import Session
from tempfile import mkdtemp
from datetime import datetime


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
@app.route('/', methods=['GET'])
def index():
  user_id = 1 #! HARD-CODED user_id
  
  # Query goals
  goals = Goals.query.all()
  return render_template("index.html", goals=goals)


@app.route('/add_goal', methods=['GET', 'POST'])
def add_goal():
  user_id = 1 #! HARD-CODED user_id
  if request.method == "POST":
    #TODO: validate inputs on the front-end
    title = request.form.get('title')
    description = request.form.get('description')
    completions_required = request.form.get('completions_required')
    reward = request.form.get('reward')

    # Add goal to database
    new_goal = Goals(title=title, description=description, num_of_completions_required=completions_required, reward=reward, user_id=user_id)
    try:
      db.session.add(new_goal)
      db.session.commit()
      return redirect('/')
    except:
      return "Failed to add goal to database"

  return render_template("add_goal.html")


@app.route('/update_goal/<int:id>', methods=['GET', 'POST'])
def update_goal(id):
  # Query goal (if it exist)
  goal = Goals.query.get_or_404(id) 

  if request.method == "POST":
    #TODO: validate inputs on the front-end
    # Update goal to database
    goal.title = request.form.get('title')
    goal.description = request.form.get('description')
    goal.completions_required = request.form.get('completions_required')
    goal.num_of_completed = request.form.get('times_completed')
    goal.reward = request.form.get('reward')

    # Check if checkbox was checked
    if request.form.get('is_completed'):
      goal.is_completed = True     
    else: # form returned NONE
      goal.is_completed = False  

    try:
      db.session.commit()
      return redirect('/')
    except:
      return 'Failed to update goal'

  return render_template("update_goal.html", goal=goal)


@app.route('/delete_goal/<int:id>', methods=['GET', 'POST'])
def delete_goal(id):
  goal = Goals.query.get_or_404(id) 

  if request.method == "POST":
    try:
      db.session.delete(goal)
      db.session.commit()
      return redirect('/')
    except:
      return 'Failed to delete goal'

  return render_template("delete_goal.html", goal=goal)


@app.route('/increment/<int:id>', methods=['GET'])
def increment(id):
  goal = Goals.query.get_or_404(id)

  if goal.is_completed:
    return 'Goal was already marked complete'
  else:
    goal.num_of_completed += 1

  # Completions required has been reached, set the goal completion status
  if goal.num_of_completed == goal.num_of_completions_required:
    goal.is_completed = True

  try:
    db.session.commit()
    return redirect('/')
  except:
    return 'Failed to increment goal'


#TODO: Create Register route
#TODO: Create Login route
#TODO (optional): Allow user to delete their account (delete user and their goals)

if __name__ == "__main__":
  db.create_all() # create tables if they haven't already been created
  app.run(debug=True) # debug mode enabled