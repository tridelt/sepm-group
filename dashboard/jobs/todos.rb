
max_length = 10

source_dir = '/var/lib/jenkins/jobs/SEPM group/workspace/'
if Dir.exists?('/home/mononofu/Programmieren/TU/SEPM-group/')
  source_dir = '/home/mononofu/Programmieren/TU/SEPM-group/'
end


SCHEDULER.every '3m', :first_in => 0 do |job|
  todos = Array.new

  Dir.glob("#{source_dir}**/*") { |fn|
    if !fn.include?('dependencies') and !fn.include?('backward.hpp') and
      !fn.include?('doxygen') and !fn.include?('dashboard') and File.file?(fn)
      File.open(fn) { |file|
        lineno = 1
        while (line = file.gets)
          if line.include?('TODO')
            todos.push({
              label: "#{File.basename(fn)}:#{lineno}",
              value: line.strip!
            })
          end
          lineno = lineno + 1
        end
      }
    end
  }


  send_event('todos', { items: todos.slice(0, max_length) })

end # SCHEDULER
