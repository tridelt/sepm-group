cd /home/mononofu/sepm-group/dashboard
case "$1" in
  "start")
    dashing job commits EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    dashing job todos EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    dashing job cppcheck EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    ;;
  "end")
    dashing job tests EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    dashing job build_status EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN
    ;;
esac

