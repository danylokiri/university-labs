
array = Array.new(3) {Array.new(4)}
# array = [[0.0, 2.0, 1.0, 1.0], [2.0, 4.0, 2.0, 3.0], [3.0, 6.0, 3.0, 5.0]]

def inc_array(array)
  (0...array.size).each do |i|
    puts "#{i + 1}-е рівняння"
    (0...array[i].size).each do |j|
      if j == 3
        print "Введіть значення після '=': "
      else
        print "Введіть #{j + 1}-й доданок: "
      end
      array[i][j] = gets.chomp.to_f
    end
  end
end

def det(arr)
  arr2 = arr[0][0] * arr[1][1] * arr[2][2] +
    arr[0][1] * arr[1][2] * arr[2][0] +
    arr[1][0] * arr[2][1] * arr[0][2] -
    arr[2][0] * arr[1][1] * arr[0][2] -
    arr[0][1] * arr[1][0] * arr[2][2] -
    arr[0][0] * arr[2][1] * arr[1][2]
  puts "Визначник матриці = #{arr2}"
end

inc_array(array)

puts "Двовимірний масив після ініціалізації: #{array.inspect}"

while det(array) == 0 || array[0][0] == 0 || array[1][1] == 0 || array[2][2] == 0
  puts "Детермінант матриці дорівнює 0, або один із елементів головної діагоналі дорівнює 0, ініціалізуйте матрицю повторно!"
  inc_array(array)
end

if array[0][0] != 1
  divider = array[0][0].dup
  (0...array[0].size).each do |j|
    array[0][j] /= divider
  end
end

puts "Двовимірний масив після першого ділення: #{array.inspect}"

if array[1][0] != 0
  multiplier = array[1][0].dup
  newRow = array[0].map { |x| x * multiplier}
  (0...array[1].size).each do |j|
    array[1][j] -= newRow[j]
  end
end

puts "Двовимірний масив після віднімання другого рядка від першого: #{array.inspect}"


if array[2][0] != 0
  multiplier = array[2][0].dup
  newRow = array[0].map { |x| x * multiplier}
  (0...array[2].size).each do |j|
    array[2][j] -= newRow[j]
  end
end

puts "Двовимірний масив після віднімання другого рядка від першого: #{array.inspect}"

if array[1][1] != 1
  divider = array[1][1].dup
  (0...array[1].size).each do |j|
    array[1][j] /= divider
  end
end

puts "Двовимірний масив після другого ділення: #{array.inspect}"

if array[0][1] != 0
  multiplier = array[0][1].dup
  newRow = array[1].map { |x| x * multiplier}
  (0...array[0].size).each do |j|
    array[0][j] -= newRow[j]
  end
end

puts "Двовимірний масив після віднімання першого рядка від другого: #{array.inspect}"

if array[2][1] != 0
  multiplier = array[2][1].dup
  newRow = array[1].map { |x| x * multiplier}
  (0...array[2].size).each do |j|
    array[2][j] -= newRow[j]
  end
end

puts "Двовимірний масив після віднімання третього рядка від другого: #{array.inspect}"

if array[2][2] != 1
  divider = array[2][2].dup
  (0...array[2].size).each do |j|
    array[2][j] /= divider
  end
end

puts "Двовимірний масив після третього ділення: #{array.inspect}"

if array[0][2] != 0
  multiplier = array[0][2].dup
  newRow = array[2].map { |x| x * multiplier}
  (0...array[0].size).each do |j|
    array[0][j] -= newRow[j]
  end
end

puts "Двовимірний масив після віднімання першого рядка від третього: #{array.inspect}"

if array[1][2] != 0
  multiplier = array[1][2].dup
  newRow = array[2].map { |x| x * multiplier}
  (0...array[1].size).each do |j|
    array[1][j] -= newRow[j]
  end
end

puts "Двовимірний масив після віднімання першого рядка від другого: #{array.inspect}"

(0...array.size).each do |j|
  puts "Відповідь на #{j + 1}-е рівняння = #{array[j][3].round(2)}"
end