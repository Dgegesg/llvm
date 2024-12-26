from flask import Flask

# Initialize the Flask application
app = Flask(__name__)

# Root URL
@app.route('/')
def home():
    return 'Welcome to the Home Page!'

# About page
@app.route('/about')
def about():
    return 'This is the About Page.'

# Contact page
@app.route('/contact')
def contact():
    return 'This is the Contact Page. Reach us at contact@example.com'

# Services page
@app.route('/services')
def services():
    return 'Here are our services.'

# Profile page with a variable in the URL (e.g., username)
@app.route('/profile/<username>')
def profile(username):
    return f"Welcome to {username}'s Profile Page!"

# Return JSON data (Example for APIs)
@app.route('/api/data')
def api_data():
    return {"message": "This is a sample API response", "status": "success"}

# Running the app
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000, debug=False)
