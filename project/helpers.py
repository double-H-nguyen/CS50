from flask import Flask, redirect, session, render_template
from functools import wraps


def login_required(f):
  """
  Decorate routes to require login.
  https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
  """
  @wraps(f)
  def decorated_function(*args, **kwargs):
    if session.get("user_id") is None:
      return redirect("/login")
    return f(*args, **kwargs)
  return decorated_function


def error(message, code=400):
  """Render error message"""
  return render_template("error.html", msg=message, code=code)


#*******************************************
#* Data Validation
#*******************************************
def add_goal_validation(title, description, completions_required, reward):
  # Title
  if not title: # check if blank
    return False, "Title field cannot be blank"
  
  # Description
  if not description:
    return False, "Description field cannot be blank"

  # Completions required
  if not completions_required:
    return False, "Number of completions required field cannot be blank"
  try:
    completions_required = int(completions_required)
  except ValueError:
    return False, "Number of completions required field can only include whole numbers"
  if completions_required <= 0:
    return False, "Number of completions required field must be a positive value"

  # Reward
  if not reward:
    return False, "Reward field cannot be blank"

  return True, ""
  

def update_goal_validation(title, description, completions_required, num_of_completed, reward):
  # Title
  if not title: # check if blank
    return False, "Title field cannot be blank"
  
  # Description
  if not description:
    return False, "Description field cannot be blank"

  # Completions required
  if not completions_required:
    return False, "Number of completions required field cannot be blank"
  try:
    completions_required = int(completions_required)
  except ValueError:
    return False, "Number of completions required field can only include whole numbers"
  if completions_required <= 0:
    return False, "Number of completions required field must be a positive value"

  # Number of times completed
  if not num_of_completed:
    return False, "Number of times completed field cannot be blank"
  try:
    num_of_completed = int(num_of_completed)
  except ValueError:
    return False, "Number of times completed field can only include whole numbers"
  if num_of_completed < 0:
    return False, "Number of times completed field must be a positive value"

  # Reward
  if not reward:
    return False, "Reward field cannot be blank"
  
  return True, ""


def login_validation(username, password):
  # Username
  if not username:
    return False, "Username field cannot be blank"

  # Password
  if not password:
    return False, "Password field cannot be blank"

  return True, ""


def register_validation(username, password, confirmation):
  # Username
  if not username:
    return False, "Username field cannot be blank"

  # Password
  if not password:
    return False, "Password field cannot be blank"

  # Confirmation
  if not confirmation:
    return False, "Confirmation field cannot be blank"

  # Check password and confirmation match
  if password != confirmation:
    return False, "Passwords do not match"
  
  return True, ""