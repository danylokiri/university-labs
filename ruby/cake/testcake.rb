require "minitest/autorun"
require_relative "./3"  # подключаем файл с методом

class Test_cake < Minitest::Test
  def test_valid
    puts("input:\n.o.o....\n........\n....o...\n........\n.....o..\n........\n\n")
    cake_split(".o.o....\n........\n....o...\n........\n.....o..\n........")

    puts("input:\n........\n..o.....\n....o...\n........\n\n")
    cake_split("........\n..o.....\n....o...\n........")
  end
  def test_invalid
    puts("input:\n.o.o....\n........\n....o...\n\n")
    cake_split(".o.o....\n........\n....o...\n")

    puts("input:\n........\n........\n........\n\n")
    cake_split("........\n........\n........")
  end
end