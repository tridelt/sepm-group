DIR="$( cd "$( dirname "$0" )" && pwd )"

while inotifywait -e modify "$DIR/build/bin/server.log"; do eval "$DIR/update_dash.sh log"; done
