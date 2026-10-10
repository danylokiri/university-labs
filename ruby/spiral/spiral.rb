def spiral(n)
  if n == "exit"
    return "ex"
  elsif n !~ /^\d+$/ || n.to_i <= 0
    return "err"
  end
  n = n.to_i
  area = n*n
  spiral = Array.new(n) { Array.new(n, 0) }
  if n % 2 != 0
    x = n/2
    y = n/2
  else
    x = n/2-1
    y = n/2-1
  end

  spiral[y][x] = 1
  i = 2
  step = 1
  while i <= area
                                   #right
    for k in 1..step
      x = x+1
      if x >= n
        x = n-1
        break
      end
      if spiral[y][x] == 0
        spiral[y][x] = i
        i = i+1
      end
    end
                                       #down
    for k in 1..step
      y = y+1
      if y >= n
        y = n-1
        break
      end
      if spiral[y][x] == 0
        spiral[y][x] = i
        i = i+1
      end
    end
    step = step + 1
                                         #left
    for k in 1..step
      x = x-1
      if x < 0
        x = 0
        break
      end
      if spiral[y][x] == 0
        spiral[y][x] = i
        i = i+1
      end
    end

                                   #up
    for k in 1..step
      y = y-1
      if y < 0
        y = 0
        break
      end
      if spiral[y][x] == 0
        spiral[y][x] = i
        i = i+1
      end
    end
    step = step + 1
  end
  return spiral
end

def main
  loop do
    puts("Введіть розмір спіральної матриці or exit")
    n = gets.chomp
    sp = spiral(n)
    
    case sp
    when "ex"
      break
    when "err"
      puts("Помилка! Будь ласка, введіть ціле число більше нуля")
    else
      for row in spiral(n)
        puts row.join(" ")
      end
    end
  end
end

if __FILE__ == $0
  main
end
