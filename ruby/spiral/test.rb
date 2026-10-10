require "minitest/autorun"
require_relative "./spiral.rb"

class SpiralTest < Minitest::Test 
	def test_valid
		assert spiral("1") == [[1]]
		assert spiral("2") == [[1, 2], [4, 3]]
		assert spiral("3") == [[7, 8, 9], [6, 1, 2], [5, 4, 3]]
		assert spiral("4") == [[7, 8, 9, 10], [6, 1, 2, 11], [5, 4, 3, 12], [16, 15, 14, 13]]
		assert spiral("exit") == "ex"
	end
	def test_invalid
		refute spiral("-10") != "err"
		refute spiral("10.5") != "err"
		refute spiral("abcd") != "err"
		refute spiral("0") != "err"
	end
end