require 'dashing'

configure do
  set :auth_token, 'EeLg5ZnXHTMeygHF7DVxptN9A56UxsXu4pN'
  set :default_dashboard, 'dash'

  helpers do
    def protected!
     # Put any authentication code you want in here.
     # This method is run before accessing any resource.
    end
  end
end

map Sinatra::Application.assets_prefix do
  run Sinatra::Application.sprockets
end

run Sinatra::Application
