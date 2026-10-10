require "minitest/autorun"
require_relative "./ipv4"  # подключаем файл с методом

class Test_ipv4 < Minitest::Test
  def test_valid
    assert check("192.168.1.1")
    assert  check("255.255.255.255")
    assert  check("0.0.0.0")
  end
  def test_invalid
    refute  check("256.256.256.256")
    refute  check("6.6.6.6.6")
    refute  check("192.168.1")
    refute  check("192.168.1.a")
  end
  def test_leading_zero
    refute check("192.168.01.1")
  end
end
