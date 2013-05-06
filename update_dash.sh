DIR="$( cd "$( dirname "$0" )" && pwd )"

if [ -d "/home/mononofu/sepm-group/" ]; then
  cd /home/mononofu/sepm-group/dashboard
else
  cd "$DIR/dashboard"
fi

case "$1" in
  "start")
    dashing job build_status EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    dashing job commits EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    dashing job todos EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    dashing job cppcheck EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    ;;
  "end")
    dashing job tests EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    dashing job build_status EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    ;;
  "log")
    dashing job log EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    ;;
esac
exit 0
