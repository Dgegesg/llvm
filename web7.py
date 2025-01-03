from flask import Flask

# Initialize the Flask application
app = Flask(__name__)

# Define the route for the main page ("/")
@app.route('/')
def hello_world():
    return "Hello, World!"

# Run the app if this script is executed directly
if __name__ == '__main__':
    app.run(debug=True)
