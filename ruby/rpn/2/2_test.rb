require "minitest/autorun"
require_relative "./2.rb"

class ReversePolishNotationTest < Minitest::Test
  def test_simple_expression
    result = main_func("3 + 4")
    assert_equal "3 4 +", result
  end

  def test_expression_with_multiple_operations
    result = main_func("3 + 4.6 * 2")
    assert_equal "3 4.6 2 * +", result
  end

  def test_division_by_zero
    result = main_func("10 / 0")
    assert_equal "Помилка! Ділити на нуль неможна", result
  end

  def test_double_symbol_error
    result = main_func("12++3")
    assert_equal "Помилка! Подвійний символ", result
  end

  def test_complex_expression
    result = main_func("5 + -3 * 6 - 4 / 2")
    assert_equal "5 -3 6 * + 4 2 / -", result
  end
end
