require 'rexml/document'

max_length = 25
build_dir = "/var/lib/jenkins/jobs/sepm/builds"

SCHEDULER.every '60m', :first_in => 0 do |job|

  build_numbers = Array.new
  Dir.glob("#{build_dir}/*") { |fn|
    build_name = File.basename(fn)
    if(build_name =~ /^\d+$/)
      build_numbers.push(Integer(build_name))
    end
  }

  tests = Array.new
  no_tests = 0
  no_failed = 0

  build_numbers.sort.each{ |build_num|
    if File.exists?("#{build_dir}/#{build_num}/junitResult.xml")
      File.open("#{build_dir}/#{build_num}/junitResult.xml") { |file|
        contents = file.read
        no_tests = contents.scan("<case>").length
        no_failed = contents.scan("<errorDetails>").length
      }
    end
    tests.push({
      name: build_num,
      total: no_tests,
      failed: no_failed
    })
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


  send_event('tests', { points: failed, more_points: succeeded })

end # SCHEDULER
