require "minitest/autorun"
require_relative "./exam"

class TestStatistics < Minitest::Test
  def test_average_with_numbers
    stats = Statistics.new([1, 2, 3, 4, 5])
    assert_equal 3.0, stats.average
  end

  def test_median_with_odd_count
    stats = Statistics.new([3, 1, 5])
    assert_equal 3, stats.median
  end

  def test_median_with_even_count
    stats = Statistics.new([3, 1, 4, 2])
    assert_equal 2.5, stats.median
  end


  def test_mode_with_single_mode
    stats = Statistics.new([1, 2, 2, 3, 4])
    assert_equal [2], stats.mode
  end

  def test_mode_with_multiple_modes
    stats = Statistics.new([1, 2, 2, 3, 3])
    assert_equal [2, 3], stats.mode
  end

  def test_mode_with_no_repeats
    stats = Statistics.new([1, 2, 3, 4, 5])
    assert_equal [1, 2, 3, 4, 5], stats.mode
  end


end
