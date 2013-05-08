from flask import Flask
import commands

app = Flask(__name__)


@app.route('/git/', methods=['POST'])
def hello_world():
  print "got hook"
  print commands.getstatusoutput('cd /home/mononofu/sepm-group/ && git pull && git push redmine')

  return 'Hello World!'

if __name__ == '__main__':
  app.run(port=8585, host='0.0.0.0')

