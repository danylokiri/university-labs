require "faraday"
require "json"
require "time"

$key = "c7cac9c99d30354e95ba29503b470a51"
$url = "https://api.openweathermap.org"
CACHE = {}

def take_inf
  if CACHE[:weather_data] && Time.now - CACHE[:timestamp] < 600
    return CACHE[:weather_data]
  end
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

    CACHE[:weather_data] = descript
    CACHE[:timestamp] = Time.now
  else
    descript = "Ошибка запроса: #{response.status}"
  end
  descript
end

def time
  start_time = Time.now
  inf_res = take_inf
  puts(inf_res)
  end_time = Time.now
  puts("time: #{end_time - start_time} sec")
end

for i in 0..10
  time
end
