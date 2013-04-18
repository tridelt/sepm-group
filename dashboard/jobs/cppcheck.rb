require 'net/http'
require 'rexml/document'

max_length = 10

source_dir = '/var/lib/jenkins/jobs/sepm/workspace/'
if Dir.exists?('/home/mononofu/Programmieren/TU/SEPM-group/')
  source_dir = '/home/mononofu/Programmieren/TU/SEPM-group/'
end



SCHEDULER.every '10s', :first_in => 0 do |job|
  issues = Array.new

  # get the XML data as a string
  xml_data = File.read(source_dir + "build/style_issues.xml")

  # extract event information
  doc = REXML::Document.new(xml_data)
  doc.elements.each("results/errors/error") { |elem|
    filename = ""
    if !elem.elements["location"].nil?
      fn = elem.elements["location"]
      filename = "#{File.basename(fn.attributes["file"])}:#{fn.attributes["line"]}"
    end

    msg = elem.attributes["msg"]
    if msg.length > 55
      msg = msg[0, 55] + "..."
    end

    issues.push({
      label: filename,
      value: msg
    })
  }

  print issues
  send_event('issues', { items: issues.slice(0, max_length) })

end # SCHEDULER
