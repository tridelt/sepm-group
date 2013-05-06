
max_length = 15

source_dir = '/var/lib/jenkins/jobs/sepm/workspace/'
if Dir.exists?('/home/mononofu/Programmieren/sepm-group_local/')
  source_dir = '/home/mononofu/Programmieren/sepm-group_local/'
end


SCHEDULER.every '60m', :first_in => 0 do |job|
  log = Array.new

  File.open(source_dir + "build/bin/server.log") { |file|
    while (line = file.gets)
      if line.length < 5
        next
      end

      label, value = line.split(']')

      if value.length > 71
        value = value[0, 70] + "..."
      end

      log.push({
          label: label + ']',
          value: value.strip!
        })
    end
  }

  log = log.reverse
  print log
  send_event('log', { items: log.slice(0, max_length) })

end # SCHEDULER
