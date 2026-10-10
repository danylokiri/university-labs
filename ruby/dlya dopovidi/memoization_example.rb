require "time"

def decorator(func)
  dictionary = {}
  -> do
    if dictionary.key?(:result)
      dictionary[:result]
    else
      dictionary[:result] = func.call
    end
  end
end

heavy = decorator(lambda {
  res = nil
  for i in 0..500000
    res = (1560000 ** 2).to_f / (789654 ** 21).to_f
  end
  res
})

def heavy_time(heavy_func)
  start_time = Time.now
  heavy_res = heavy_func.call
  puts(heavy_res)
  end_time = Time.now
  puts("time: #{end_time - start_time} sec")
end

for i in 0..10
  heavy_time(heavy)
end
