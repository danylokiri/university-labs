def check(input)
  number = input.split('.')
  if number.size != 4
    return false
  end
  for i in number
    if i != i.to_i.to_s                #проверка на 0 в начале, цифры и т.д
      return false
    elsif i.to_i < 0 || i.to_i > 255
      return false
    end
  end
  return true
end

def main
 loop do
  input = gets.chomp
  if check(input)
    puts("true")
  else
    puts("false")
  end
 end
end

if __FILE__ == $0
  main
end