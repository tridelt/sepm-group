
max_length = 10

SCHEDULER.every '3m', :first_in => 0 do |job|
  todos = Array.new

  Dir.glob('/home/mononofu/Programmieren/TU/SEPM-group/**/*') { |fn|
    if !fn.include?('dependencies') and !fn.include?('backward.hpp') and
      !fn.include?('Doxyfile') and !fn.include?('dashboard') and File.file?(fn)
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
