require 'net/http'
require 'rexml/document'

# Web search for "madonna"
url = 'http://ec2.furidamu.org/cc.xml'


SCHEDULER.every '10s', :first_in => 0 do |job|
  # get the XML data as a string
  xml_data = Net::HTTP.get_response(URI.parse(url)).body

  # extract event information
  doc = REXML::Document.new(xml_data)
  status = doc.elements["//Project[@name='SEPM group']"]
        .attributes["lastBuildStatus"]

  activity = doc.elements["//Project[@name='SEPM group']"]
        .attributes["activity"]

  if activity == "Sleeping"
    if status == 'Success'
      status = 'ok'
      activity = "Succeeded"
    else
      status = 'warning'
      activity = "Failed"
    end
  else
    status = "building"
  end

  send_event('build_status', { text: activity, status: status })

end # SCHEDULER
