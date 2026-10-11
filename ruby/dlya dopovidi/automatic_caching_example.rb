require "faraday"
require "json"
require "time"
require "active_support/cache"
require "active_support/notifications"

$key = ENV.fetch("OPENWEATHER_API_KEY") # set your own OpenWeatherMap API key in the environment
$url = "https://api.openweathermap.org"


cache = ActiveSupport::Cache::MemoryStore.new

def take_inf(cache)
  cache.fetch("weather_info", expires_in: 300) do
    conn = Faraday.new(url: $url)
    response = conn.get('/data/2.5/weather') do |req|
      req.params['appid'] = $key
      req.params['units'] = 'metric'
      req.params['lang'] = 'ua'
      req.params['q'] = 'skien, no'
      req.params['limit'] = 1
    end

    if response.status == 200
      weather = JSON.parse(response.body)
      descript = weather["weather"][0]["description"]
    else
      descript = "Ошибка запроса: #{response.status}"
    end
    descript
  end
end

def time(cache)
  start_time = Time.now
  inf_res = take_inf(cache)
  puts(inf_res)
  end_time = Time.now
  puts("time: #{end_time - start_time} sec")
end

for i in 0..10
  time(cache)
end
