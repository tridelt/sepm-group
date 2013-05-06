#!/usr/bin/env ruby
require 'net/http'
require 'json'
require 'date'

# This job can track metrics of a public visible user or organisation’s repos
# by using the public api of github.
#
# Note that this API only allows 60 requests per hour.
#
# This Job should use the `List` widget

# Config
# ------
# example for tracking single user repositories
# github_username = 'users/ephigenia'
# example for tracking an organisations repositories
github_username = ENV['GITHUB_USER_REPOS_USERNAME'] || 'sepm-depm'
github_repo = 'sepm-group'
# number of repositories to display in the list
max_length = 7
# order the list by the numbers
ordered = true

SCHEDULER.every '60m', :first_in => 0 do |job|
  http = Net::HTTP.new("api.github.com", Net::HTTP.https_default_port())
  http.use_ssl = true
  response = http.request(Net::HTTP::Get.new("/repos/#{github_username}/#{github_repo}/commits"))

  data = JSON.parse(response.body)

  if response.code != "200"
    puts "github api error (status-code: #{response.code})\n#{response.body}"
  else
    commits = Array.new
    repos_watchers = Array.new

    data.each do |commit|
      raw_date = commit['commit']['author']['date']
      time = DateTime.parse(raw_date).new_offset(Rational(2, 24)).strftime('%H:%M')
      msg = commit['commit']['message']
      if msg.length > 50
        msg = msg[0, 50] + "..."
      end
      commits.push({
        label: time + ", " + commit['commit']['author']['name'],
        value: msg,
        date: raw_date
      })
    end

    if ordered
      commits = commits.sort_by { |com| com[:date] }.reverse
    end

    send_event('github_repo_commits', { items: commits.slice(0, max_length) })

  end # if

end # SCHEDULER
