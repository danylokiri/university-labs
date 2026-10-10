require "faraday"
require "json"
require "csv"

#https://openweathermap.org/api/geocoding-api, https://openweathermap.org/current#parameter, https://lostisland.github.io/faraday/#/getting-started/quick-start, https://ruby-doc.org/stdlib-2.6.1/libdoc/csv/rdoc/CSV.html
def take_inf(city, country)
  key = "c7cac9c99d30354e95ba29503b470a51"
  conn = Faraday.new(
    url: "https://api.openweathermap.org",
    params: { appid: key, units: "metric", lang: "ua" },
  )
  response = conn.post('/data/2.5/weather') do |req|
    req.params['q'] = city + "," + country
    req.params["limit"] = 1
  end
  if response.status == 200
    weather = JSON.parse(response.body)
    name = weather["name"]
    descript = weather["weather"][0]["description"]
    temp = weather["main"]["temp"]
    feels = weather["main"]["feels_like"]
    wind = weather["wind"]["speed"]

    csv_save(name, descript, temp, feels, wind)
  end
  response

end

def csv_save(name, descript, temp, feels, wind)
  CSV.open("info.csv", "w",write_headers: true, headers: ["Місто", "Погода", "Температура", "Відчуваєтсья як", "Швидкість вітру"]) do |csv|           #w - запись, w+ запись и чтение (тоже с перезаписью), a+ добавляние и чтение
    csv << [name, descript, temp, feels, wind]
  end
end
def main
  loop do
    puts("введіть місто англійською або exit")
    city = gets.chomp.downcase
    if city == "exit"
      break
    else
      puts("введіть код країни або exit")
      country = gets.chomp.downcase
      if country == "exit"
        break
        else
      take_inf(city, country)
      end
    end
  end
end

if __FILE__ == $0
  main
end