require "minitest/autorun"

require_relative "./api"  # подключаем файл с методом

class Test_ipv4 < Minitest::Test
  def test_connection
    response = take_inf("kyiv", "ua")
    assert_equal 200, response.status

    response = take_inf("kyitsd1", "uk")
    refute_equal 200, response.status
  end

  def test_correct
    response = take_inf("kyiv", "ua")
    weather = JSON.parse(response.body)

    refute_equal weather["name"], nil
    refute_equal weather["weather"][0]["description"], nil
    refute_equal weather["main"]["temp"], nil
    refute_equal weather["main"]["feels_like"], nil
    refute_equal weather["wind"]["speed"], nil
    assert_instance_of String, weather["name"]
    assert_instance_of String, weather["weather"][0]["description"]
    assert_instance_of Float, weather["main"]["temp"]
    assert_instance_of Float, weather["main"]["feels_like"]
    assert_instance_of Float, weather["wind"]["speed"]
  end

  def test_csv_created
    response = take_inf("skien", "no")
    weather = JSON.parse(response.body)
    csv = CSV.read("info.csv", headers: true)
    headers = ["Місто", "Погода", "Температура", "Відчуваєтсья як", "Швидкість вітру"]
    assert_path_exists "info.csv"
    assert_equal headers, csv.headers
    assert_equal weather["name"], csv[0]["Місто"]
    assert_equal weather["weather"][0]["description"], csv[0]["Погода"]
    assert_equal weather["main"]["temp"].to_s, csv[0]["Температура"]          #.to_s в cvs ток строки
    assert_equal weather["main"]["feels_like"].to_s, csv[0]["Відчуваєтсья як"]
    assert_equal weather["wind"]["speed"].to_s, csv[0]["Швидкість вітру"]
  end
end