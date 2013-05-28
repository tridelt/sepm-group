
max_length = 7

source_dir = '/var/lib/jenkins/jobs/sepm/workspace/'
if Dir.exists?('/home/mononofu/Programmieren/sepm-group/')
  source_dir = '/home/mononofu/Programmieren/sepm-group/'
end


SCHEDULER.every '60m', :first_in => 0 do |job|
  todos = Array.new

  Dir.glob("#{source_dir}**/*") { |fn|
    if !fn.include?('dependencies') and !fn.include?('backward.hpp') and
      !fn.include?('doxygen') and !fn.include?('dashboard') and
      !fn.include?('build') and !fn.include?('cmake') and File.file?(fn)
      File.open(fn) { |file|
        lineno = 0
        while (line = file.gets)
          lineno = lineno + 1

          if line.nil?
            next
          end

          if line.include?('TODO')
            line = line[line.index('TODO') + 5, line.length]
          elsif line.include?('FIXME')
            line = line[line.index('FIXME') + 6, line.length]
          elsif line.include?('BUG')
            line = line[line.index('BUG') + 4, line.length]
          else
            next
          end

          line = line.strip!

          if line.length > 55
            line = line[0, 55] + "..."
          end

          todos.push({
              label: "#{File.basename(fn)}:#{lineno}",
              value: line
            })
        end
      }
    end
  }

  print todos
  send_event('todos', { items: todos.slice(0, max_length) })

end # SCHEDULER
