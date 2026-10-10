class Statistics
  def initialize(numbers = [])
    @numbers = numbers
  end

  def average
    @numbers.sum / @numbers.size.to_f
  end

  def median
    sorted = @numbers.sort
    if sorted.size % 2 == 1
      sorted[sorted.size / 2]
    else
      (sorted[sorted.size / 2 - 1] + sorted[sorted.size / 2]) / 2.0
    end
  end

  def mode
    count = @numbers.tally
    mode = []
    max_count = count.values.max
    for key, value in count
      if value == max_count
        mode.push(key)
      end
    end
    mode
  end
end

def main
  loop do
    puts "\nMenu\n1. Enter numbers manually\n2. Generate random numbers\n0. Exit\n"
    choice = gets.chomp

    if choice == "1"
      print "Enter numbers separated by commas:"
      numbers = gets.chomp.split(",").map(&:to_i)
      if numbers.size == 0
        puts("error (0 symbols cant be accepted)")
      end
      stats = Statistics.new(numbers)
      puts "Average: #{stats.average}"
      puts "Median: #{stats.median}"
      puts "Mode: #{stats.mode.inspect}"

    elsif choice == "2"
      print "Enter the number of random numbers:"
      count = gets.chomp.to_i
      numbers = []
      for i in 0...count
        numbers.push(rand(1..5))
      end
      if numbers.size == 0
        puts("error (0 symbols cant be accepted)")
      end
      puts numbers.inspect
      stats = Statistics.new(numbers)
      puts "Average: #{stats.average}"
      puts "Median: #{stats.median}"
      puts "Mode: #{stats.mode.inspect}"
    elsif choice == "0"
      exit

    else
      puts "error"
      exit
    end
  end
end

if __FILE__ == $0
  main
end