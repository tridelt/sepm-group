require 'rexml/document'

max_length = 25

SCHEDULER.every '60m', :first_in => 0 do |job|
  tests = Array.new
  no_tests = 0
  no_failed = 0

  Dir.glob("/var/lib/jenkins/jobs/sepm/builds/*") { |fn|
    build_name = File.basename(fn)
    if(build_name =~ /^\d+$/)
      if File.exists?("#{fn}/junitResult.xml")
        File.open("#{fn}/junitResult.xml") { |file|
          contents = file.read
          no_tests = contents.scan("<case>").length
          no_failed = contents.scan("<errorDetails>").length
        }
      end
      tests.push({
        name: Integer(build_name),
        total: no_tests,
        failed: no_failed
      })
    end
  }

  tests = tests.sort_by { |t| t[:name] }

  succeeded = Array.new
  failed = Array.new

  tests.each{ |t|
    succeeded.push({
      x: t[:name],
      y: t[:total] - t[:failed]
    })
    failed.push({
      x: t[:name],
      y: t[:failed]
    })
  }

  succeeded = succeeded.slice([0, succeeded.length - max_length].max,
                              max_length)
  failed = failed.slice([0, failed.length - max_length].max,
                              max_length)


  send_event('tests', { points: succeeded, more_points: failed })

end # SCHEDULER
