require "time"
require "memoist"

class Calculator
  extend Memoist
  def heavy
    res = nil
    for i in 0..500000
      res = (1560000 ** 2).to_f / (789654 ** 21).to_f
    end
    res
  end

  memoize :heavy
  def heavy_time
    start_time = Time.now
    heavy_res = heavy
    puts(heavy_res)
    end_time = Time.now
    puts("time: #{end_time - start_time} sec")
  end
end

calculator = Calculator.new
for i in 0..10
  calculator.heavy_time
end
