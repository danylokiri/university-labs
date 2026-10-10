def cake_split(input)
  cake = []
  rows = input.split("\n")
  for i in 0...rows.size            # .. для [], ... для [)
    cols = rows[i].split("")
    cake.push(cols)
  end

  rodzinki = []
  for x in 0...cake.size
    for y in 0...cake[0].size
      if cake[x][y] == "o"
        rodzinki.push([x, y])
      end
    end
  end
  if rodzinki.size <= 1 || rodzinki.size > 10
    puts "Не вірна кількість родзинок!"
    return
  end

  total_size = cake.size * cake[0].size
  if total_size % rodzinki.size != 0
    puts "Помилка! Цей пиріг не ділиться на рівні частини"
    return
  end

  piece_size = total_size / rodzinki.size
  zanyato = []
  for i in 0...cake.size
    row = []
    for j in 0...cake[0].size
      row.push(false)
    end
    zanyato.push(row)
  end

  pieces = []
  for i in 0...rodzinki.size
    x = rodzinki[i][0]
    y = rodzinki[i][1]
    piece = found_piece(x, y, cake, piece_size, zanyato)
    if piece
      pieces.push(piece)
    else
      puts "Помилка! Цей пиріг не  можна поділити націло по одній родзинці"
      return
    end
  end
  puts "Шматки:"
  for i in 0...pieces.size
    print_piece(pieces[i])
  end
end
def found_piece(x, y, cake, piece_size, zanyato)
  for height in 1..cake.size
    for width in 1..cake[0].size
      if width * height != piece_size
        next
      end
      for start_x in 0..(cake.size - height)
        if x < start_x || x >= start_x + height
          next
        end
        for start_y in 0..(cake[0].size - width)
          if y < start_y || y >= start_y + width
            next
          end
                                                                #puts "Проверяется кусок размером #{width}x#{height} с началом в (#{start_x}, #{start_y})" if width == 6 && height == 2
          if good_piece(start_x, start_y, width, height, cake, zanyato)
            add_zanyato(start_x, start_y, width, height, zanyato)
            return create_piece(start_x, start_y, width, height, cake)
          end
        end
        for start_x in 0..(cake.size - width)
          for start_y in 0..(cake[0].size - height)
            if good_piece(start_x, start_y, height, width, cake, zanyato)
              add_zanyato(start_x, start_y, height, width, zanyato)
              return create_piece(start_x, start_y, height, width, cake)
            end
          end
        end
      end
    end
  end
  return nil
end

def good_piece(start_x, start_y, width, height, cake, zanyato)
  count_rodzinki = 0
  for i in start_x...(start_x + height)
    for j in start_y...(start_y + width)
      if zanyato[i][j]
        return false
      end
      if cake[i][j] == "o"
        count_rodzinki += 1
      end
    end
  end
  return count_rodzinki == 1
end

def add_zanyato(start_x, start_y, width, height, zanyato)
  for i in start_x...(start_x + height)
    for j in start_y...(start_y + width)
                                                           #puts "Помечаем занятую клетку на (#{i}, #{j})"
      zanyato[i][j] = true
    end
  end
end

def create_piece(start_x, start_y, width, height, cake)
  piece = []
  for i in start_x...(start_x + height)
    row = []
    for j in start_y...(start_y + width)
      row.push(cake[i][j])
    end
    piece.push(row)
  end
  return piece
end

def print_piece(piece)
  for i in 0...piece.size
    puts piece[i].join("")
  end
  puts ",\n"
end

def main
  puts("Введіть без пробілів пирог+END або exitEND")
  $/ = "END"
  input = STDIN.gets
  $/ = "\n"
  input.chomp!("END")
                                              puts(input.inspect)
  input.strip!
  if input.strip == "exit"
    exit
  else
    cake_split(input)
  end
end


if __FILE__ == $0
  main
end